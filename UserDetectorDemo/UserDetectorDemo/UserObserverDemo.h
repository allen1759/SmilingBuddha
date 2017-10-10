/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _USER_OBSERVER_DEMO_H
#define _USER_OBSERVER_DEMO_H

#include <iostream>

#include "../../SmilingBuddha/SmilingBuddha/UserObserver.h"

class UserObserverDemo : public UserObserver
{
public:
	UserObserverDemo();
	~UserObserverDemo();

private:

	virtual void OnUserDetect() override;

	virtual void OnUserLeave() override;

};

UserObserverDemo::UserObserverDemo()
{
}

UserObserverDemo::~UserObserverDemo()
{
}

void UserObserverDemo::OnUserDetect()
{
	std::cout << "User in..." << std::endl;
}

void UserObserverDemo::OnUserLeave()
{
	std::cout << "User out..." << std::endl;
}

#endif // !_USER_OBSERVER_DEMO_H