/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "ArduinoUserDetector.h"

#include <exception>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception_ptr.hpp>

#include <iostream>

ArduinoUserDetector * ArduinoUserDetector::instance = NULL;

ArduinoUserDetector::ArduinoUserDetector() : ioService(), serialPort(ioService)
{
	isRunning = false;
	detectUserThread = NULL;

	// TODO: read comport from file
	portName = "COM3";

	signalBuffer = new bool[DETECT_SIGNAL_WINDOW_SIZE];
	for (int i = 0; i < DETECT_SIGNAL_WINDOW_SIZE; ++i)
		signalBuffer[i] = false;
	signalBufferIndex = 0;
	detectedSignalCount = 0;
	lastDetectSignal = false;

	Start();
}

ArduinoUserDetector::~ArduinoUserDetector()
{
	isRunning = false;
	if (detectUserThread) {
		detectUserThread->join();
		detectUserThread = NULL;
	}

	delete[] signalBuffer;
}

ArduinoUserDetector * ArduinoUserDetector::GetInstance()
{
	if (instance == NULL)
		instance = new ArduinoUserDetector();

	return instance;
}

void ArduinoUserDetector::Start()
{
	if (!isRunning) {
		isRunning = true;
		detectUserThread = std::make_shared<std::thread>(&ArduinoUserDetector::DetectUser, this);
	}
}

std::string ArduinoUserDetector::ReadLine()
{
	char ch;
	std::string result;
	try {
		while (true) {
			boost::asio::read(serialPort, boost::asio::buffer(&ch, 1));
			if (ch == '\r') {
				boost::asio::read(serialPort, boost::asio::buffer(&ch, 1));
				if (ch == '\n')
					return result;
			}
			else  {
				result += ch;
			}
		}
	}
	catch (boost::exception const &e) {
		throw std::runtime_error("Error serial port read failed.");
	}
}

void ArduinoUserDetector::DetectUser()
{
	while (isRunning) {
		try {
			OpenSerialPort();
			ProcessSerialData();
		}
		catch (boost::exception const &e) {
			std::cerr << "Cannot open Arduino." << std::endl;
		}
	}
}

void ArduinoUserDetector::OpenSerialPort()
{
	serialPort.open(portName);
	serialPort.set_option(boost::asio::serial_port_base::baud_rate(BAUD_RATE));
	serialPort.set_option(boost::asio::serial_port_base::character_size(8));
	serialPort.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	serialPort.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	serialPort.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port::flow_control::none));
}

void ArduinoUserDetector::CloseSerialPort()
{
	serialPort.close();

	for (int i = 0; i < DETECT_SIGNAL_WINDOW_SIZE; ++i)
		signalBuffer[i] = false;
	signalBufferIndex = 0;
	detectedSignalCount = 0;
	lastDetectSignal = false;
}


void ArduinoUserDetector::ProcessSerialData()
{
	try {
		while (isRunning) {
			std::string line = ReadLine();
			if (line != "1" && line != "0")
				continue;

			if (signalBuffer[signalBufferIndex])
				detectedSignalCount--;

			if (line == "1") {
				signalBuffer[signalBufferIndex] = true;
				detectedSignalCount++;
			}
			else if (line == "0") 
				signalBuffer[signalBufferIndex] = false;

			signalBufferIndex = (signalBufferIndex + 1) % DETECT_SIGNAL_WINDOW_SIZE;


			bool detectSignal = detectedSignalCount >= DETECT_SIGNAL_WINDOW_SIZE_HALF;
			if (!lastDetectSignal && detectSignal) {
				if (observer)
					observer->OnUserDetect();
			}
			else if (lastDetectSignal && !detectSignal) {
				if (observer)
					observer->OnUserLeave();
			}
			lastDetectSignal = detectSignal;
		}
	}
	catch (std::exception const &e) {
		std::cerr << "Cannot read Arduino" << std::endl;
		CloseSerialPort();
	}
}
