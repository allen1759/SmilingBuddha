/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include <thread>

#include "UserObserverDemo.h"
#include "../../SmilingBuddha/SmilingBuddha/UserDetector.h"

int main()
{
	UserDetector *userDetector = UserDetector::GetInstance();
	userDetector->SetUserObserver(new UserObserverDemo());

	std::this_thread::sleep_for(std::chrono::seconds(100000));

	return 0;
}