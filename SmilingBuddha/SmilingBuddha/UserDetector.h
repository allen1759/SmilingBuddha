/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _USER_DETECTOR_H
#define _USER_DETECTOR_H

#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "UserObserver.h"

class UserDetector
{
public:
	static UserDetector * GetInstance();
	~UserDetector();

	void SetUserObserver(UserObserver *observer);


private:

	UserDetector();

	std::string ReadLine();
	void Start();
	void DetectUser();
	void OpenSerialPort();
	void CloseSerialPort();
	void ProcessSerialData();


	static UserDetector *instance;

	bool isRunning;
	std::shared_ptr<std::thread> detectUserThread;

	UserObserver *observer;

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



#endif // !_USER_DETECTOR_H
