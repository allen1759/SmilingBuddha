/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoClip.h"

#include <iostream>

VideoClip::VideoClip(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence, float duration)
{
	this->imageSequence = imageSequence;
	this->duration = std::chrono::milliseconds(static_cast<int>(duration * 1000));
	startTime = std::chrono::high_resolution_clock::now();
}

VideoClip::~VideoClip()
{
}

std::shared_ptr<cv::Mat> VideoClip::GetFrame()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	double delta = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count());
	int index = static_cast<int>(static_cast<double>(delta) / duration.count() * imageSequence->size());
	
	if (index < imageSequence->size())
		return imageSequence->at(index);
	else
		return imageSequence->back();
}

std::shared_ptr<Video> VideoClip::GetVideo()
{
	return std::make_shared<VideoClip>(*this);
}