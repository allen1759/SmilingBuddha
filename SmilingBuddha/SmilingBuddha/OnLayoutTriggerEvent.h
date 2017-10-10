/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ON_LAYOUT_TRIGGER_EVENT_H
#define _ON_LAYOUT_TRIGGER_EVENT_H

#include "Event.h"

#include "InteractionState.h"

class OnLayoutTriggerEvent : public Event
{
public:
	OnLayoutTriggerEvent()
	{
	}

	virtual ~OnLayoutTriggerEvent()
	{
	}

	virtual void Dispatch(std::shared_ptr<InteractionState> state) override
	{
		state->OnLayoutTrigger();
	}

private:

};

#endif // !_ON_LAYOUT_TRIGGER_EVENT_H
