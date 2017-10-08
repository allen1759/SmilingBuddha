/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "DebugUserDetector.h"

DebugUserDetector *DebugUserDetector::instance = NULL;

DebugUserDetector::DebugUserDetector()
{
	isUserDetect = false;

	detectUserThread = std::thread(&DebugUserDetector::DetectUser, this);
}

DebugUserDetector::~DebugUserDetector()
{
}

DebugUserDetector * DebugUserDetector::GetInstance()
{
	if (instance == NULL)
		instance = new DebugUserDetector();

	return instance;
}

void DebugUserDetector::DetectUser()
{
	while (true) {
		char key = InputManager::GetInstance()->GetKey();
		if (key == 'u' && observer != NULL) {
			if (!isUserDetect)
				observer->OnUserDetect();
			else
				observer->OnUserLeave();
			isUserDetect = !isUserDetect;
		}
	}
}
