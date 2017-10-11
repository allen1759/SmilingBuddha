/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ON_ROI_TRIGGER_EVENT_H
#define _ON_ROI_TRIGGER_EVENT_H

#include "Event.h"

#include "InteractionState.h"

class OnRoiTriggerEvent : public Event
{
public:
	OnRoiTriggerEvent()
	{
	}

	virtual ~OnRoiTriggerEvent()
	{
	}

	virtual void Dispatch(std::shared_ptr<InteractionState> state) override
	{
		state->OnRoiTrigger();
	}
private:

};

#endif // !_ON_ROI_TRIGGER_EVENT_H

