/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _EVENT_H
#define _EVENT_H

#include <memory>

class InteractionState;

class Event
{
public:
	virtual ~Event() {}

	virtual void Dispatch(std::shared_ptr<InteractionState> state) = 0;

};

#endif // !_EVENT_H
