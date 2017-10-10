/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ON_SMILE_EVENT_H
#define _ON_SMILE_EVENT_H

#include "Event.h"

#include "InteractionState.h"

class OnSmileEvent : public Event
{
public:
	OnSmileEvent()
	{
	}

	virtual ~OnSmileEvent()
	{
	}

	virtual void Dispatch(std::shared_ptr<InteractionState> state) override
	{
		state->OnSmile();
	}

private:


};

#endif // !_ON_SMILE_EVENT_H
