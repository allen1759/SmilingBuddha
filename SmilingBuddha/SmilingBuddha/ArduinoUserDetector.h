/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ARDUINO_USER_DETECTOR_H
#define _ARDUINO_USER_DETECTOR_H

#include "UserDetector.h"

#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "UserObserver.h"

class ArduinoUserDetector : public UserDetector
{
private:
	ArduinoUserDetector();

public:
	~ArduinoUserDetector();

	static ArduinoUserDetector * GetInstance();

private:
	std::string ReadLine();

	void Start();

	void DetectUser();

	void OpenSerialPort();

	void CloseSerialPort();

	void ProcessSerialData();


	static ArduinoUserDetector *instance;

	bool isRunning;
	std::shared_ptr<std::thread> detectUserThread;

	std::string portName;
	const int BAUD_RATE = 9600;
	boost::asio::io_service ioService;
	boost::asio::serial_port serialPort;

	const int DETECT_SIGNAL_WINDOW_SIZE = 63;
	const int DETECT_SIGNAL_WINDOW_SIZE_HALF = 32;
	bool *signalBuffer;
	int signalBufferIndex;
	int detectedSignalCount;
	bool lastDetectSignal;
};

#endif // !_ARDUINO_USER_DETECTOR_H
