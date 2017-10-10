/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ON_RECORDED_EVENT_H
#define _ON_RECORDED_EVENT_H


#include "Event.h"

#include <vector>

#include <opencv2\core\core.hpp>

#include "InteractionState.h"

class OnRecordedEvent : public Event
{
public:
	OnRecordedEvent(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
	{
		this->images = images;
	}

	virtual ~OnRecordedEvent()
	{
	}

	virtual void Dispatch(std::shared_ptr<InteractionState> state) override
	{
		state->OnRecorded(images);
	}

private:
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images;
};


#endif // !_ON_RECORDED_EVENT_H
