/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ON_USER_DETECT_EVENT_H
#define _ON_USER_DETECT_EVENT_H

#include "Event.h"

#include "InteractionState.h"

class OnUserDetectEvent : public Event
{
public:
	OnUserDetectEvent()
	{
	}

	virtual ~OnUserDetectEvent()
	{
	}

	virtual void Dispatch(std::shared_ptr<InteractionState> state) override
	{
		state->OnUserDetect();
	}

private:

};

#endif // !_ON_USER_DETECT_EVENT_H
