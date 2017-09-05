/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "InteractionState.h"

InteractionState::InteractionState(Director * director)
{
	this->director = director;
}

InteractionState::~InteractionState()
{
}

void InteractionState::OnSmile()
{}

void InteractionState::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{}

