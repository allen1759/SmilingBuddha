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
	currentFrameNumber = 0.0f;

	elapsedTime = 0;
	lastTime = std::chrono::high_resolution_clock::now();
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
	currentFrameNumber += deltaTime * framePerSecond;

	if (reverse) {
		if (currentFrameNumber >= imageSequence->size() * 2 - 1) {
			if (loop)
				currentFrameNumber = std::fmod(currentFrameNumber, imageSequence->size() * 2 - 1);
			else {
				end = true;
				currentFrameNumber = imageSequence->size() * 2 - 1;
			}
		}

		if (currentFrameNumber < imageSequence->size())
			currentFrameIndex = static_cast<int>(currentFrameNumber);
		else
			currentFrameIndex = imageSequence->size() * 2 - 1 - static_cast<int>(currentFrameNumber);
	}
	else {
		if (currentFrameNumber >= imageSequence->size() - 1) {
			if (loop)
				currentFrameNumber = std::fmod(currentFrameNumber, imageSequence->size());
			else {
				end = true;
				currentFrameNumber = imageSequence->size() - 1;
			}
		}
		currentFrameIndex = static_cast<int>(currentFrameNumber);
	}

	return imageSequence->at(currentFrameIndex);
}

std::shared_ptr<Video> VideoClip::GetVideo()
{
	return shared_from_this();
}