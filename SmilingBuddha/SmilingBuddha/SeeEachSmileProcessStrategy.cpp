/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SeeEachSmileProcessStrategy.h"

SeeEachSmileProcessStrategy::SeeEachSmileProcessStrategy(SmileObserver * observer)
{
	this->observer = observer;
}

SeeEachSmileProcessStrategy::~SeeEachSmileProcessStrategy()
{
}

void SeeEachSmileProcessStrategy::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
}