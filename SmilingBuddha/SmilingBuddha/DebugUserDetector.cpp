/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "DebugUserDetector.h"

inline DebugUserDetector::DebugUserDetector()
{
	isUserDetect = false;

	detectUserThread = std::thread(&DebugUserDetector::DetectUser, this);
}

inline DebugUserDetector::~DebugUserDetector()
{
}

inline DebugUserDetector * DebugUserDetector::GetInstance()
{
	if (instance == NULL)
		instance = new DebugUserDetector();

	return instance;
}

inline void DebugUserDetector::SetUserObserver(UserObserver * observer)
{
	this->observer = observer;
}

inline void DebugUserDetector::DetectUser()
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
