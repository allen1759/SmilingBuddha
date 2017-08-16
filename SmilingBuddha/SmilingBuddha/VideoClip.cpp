/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoClip.h"

#include <iostream>

VideoClip::VideoClip(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence)
{
	this->imageSequence = imageSequence;
}

VideoClip::~VideoClip()
{
}

std::shared_ptr<cv::Mat> VideoClip::GetFrame()
{
	std::cout << "Video Clip" << std::endl;
	return NULL;
}

Video * VideoClip::GetVideo()
{
	return this;
}