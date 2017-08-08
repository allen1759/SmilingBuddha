/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SeeEachSmileProcess.h"

SeeEachSmileProcess::SeeEachSmileProcess(SmileObserver * observer)
{
	this->observer = observer;
}

SeeEachSmileProcess::~SeeEachSmileProcess()
{
}

void SeeEachSmileProcess::InitializeProcess()
{
}

void SeeEachSmileProcess::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
}