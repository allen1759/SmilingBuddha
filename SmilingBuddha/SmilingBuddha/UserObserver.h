/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _USER_OBSERVER_H
#define _USER_OBSERVER_H

class UserObserver
{
public:
	virtual void OnUserDetect() = 0;
	virtual void OnUserLeave() = 0;

};

#endif // !_USER_OBSERVER_H
