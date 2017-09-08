/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoClip.h"

#include <iostream>
#include <cmath>

VideoClip::VideoClip(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence, float videoTime, bool loop, bool reverse)
{
	this->imageSequence = imageSequence;	
	this->loop = loop;
	this->reverse = reverse;
	end = false;

	framePerSecond = static_cast<float>(imageSequence->size()) / videoTime;
	secondsPerFrame = 1.0f / framePerSecond;
	currentFrameIndex = 0;

	elapsedTime = 0;
	startTime = std::chrono::high_resolution_clock::now();
	lastTime = startTime;
}

VideoClip::~VideoClip()
{
}

std::shared_ptr<cv::Mat> VideoClip::GetFrame()
{
	if (end)
		return imageSequence->at(currentFrameIndex);

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastTime).count();

	if (deltaTime < secondsPerFrame)
		return imageSequence->at(currentFrameIndex);

	lastTime = currentTime;
	elapsedTime += deltaTime;

	int frameCount = static_cast<int>(elapsedTime * framePerSecond);

	if (reverse)
		currentFrameIndex = std::abs(static_cast<int>((frameCount + (imageSequence->size() - 1)) % (2 * (imageSequence->size() - 1)) - (imageSequence->size() - 1)));
	else 
		currentFrameIndex = frameCount % imageSequence->size();

	if (!loop) {
		if (reverse) {
			if (frameCount >= 2 * (imageSequence->size() - 1)) {
				currentFrameIndex = 0;
				end = true;
			}
		}
		else {
			if (frameCount >= imageSequence->size() - 1) {
				currentFrameIndex = imageSequence->size() - 1;
				end = true;
			}
		}
	}
	return imageSequence->at(currentFrameIndex);
	
	
}

std::shared_ptr<Video> VideoClip::GetVideo()
{
	return shared_from_this();
}