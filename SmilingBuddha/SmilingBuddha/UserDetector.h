/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _USER_DETECTOR_H
#define _USER_DETECTOR_H

#include "UserObserver.h"

class UserDetector
{
public:
	virtual ~UserDetector()
	{}

	virtual void SetUserObserver(UserObserver *observer)
	{
		this->observer = observer;
	}

protected:
	UserObserver *observer;
};

#endif // !_USER_DETECTOR_H
