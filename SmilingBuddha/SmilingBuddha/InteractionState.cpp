/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "InteractionState.h"

#include "Director.h"
#include "PreludeInitialState.h"

InteractionState::InteractionState(Director * director)
{
	this->director = director;
}

InteractionState::~InteractionState()
{
}

void InteractionState::SignalEvent(std::shared_ptr<Event> e)
{
	e->Dispatch(shared_from_this());
}

void InteractionState::OnUserDetect()
{
}

void InteractionState::OnUserLeave()
{
	director->SetInteractionState(std::make_shared<PreludeInitialState>(director));
}

void InteractionState::OnSmile()
{
}

void InteractionState::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
}

void InteractionState::OnLayoutTrigger()
{

}
