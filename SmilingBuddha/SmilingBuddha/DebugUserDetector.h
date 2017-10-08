/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _DEBUG_USER_DETECTOR_H
#define _DEBUG_USER_DETECTOR_H

#include <thread>

#include "UserObserver.h"
#include "InputManager.h"

class DebugUserDetector
{
private:
	DebugUserDetector();

public:
	~DebugUserDetector();

	static DebugUserDetector * GetInstance();

	void SetUserObserver(UserObserver *observer);

private:
	void DetectUser();

	static DebugUserDetector *instance;

	std::thread detectUserThread;

	UserObserver *observer;
	bool isUserDetect;
};

#endif // !_DEBUG_USER_DETECTOR_H
