/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/


#ifndef _FADE_ANIMATION_H
#define _FADE_ANIMATION_H

#include "AnimatedVideo.h"


#include <opencv2/highgui/highgui.hpp>

class FadeAnimationVideo : public AnimatedVideo
{
public:
	FadeAnimationVideo(std::shared_ptr<Video> video, float duration, unsigned char r, unsigned char g, unsigned char b);
	
	virtual ~FadeAnimationVideo() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

private:
	unsigned char r;
	unsigned char g;
	unsigned char b;

	bool end;

	std::shared_ptr<cv::Mat> fadedFrame;
	std::shared_ptr<cv::Mat> currentFrame;
};

FadeAnimationVideo::FadeAnimationVideo(std::shared_ptr<Video> video, float duration, unsigned char R, unsigned char G, unsigned char B)
	: AnimatedVideo(video, duration)
{
	this->r = r;
	this->g = g;
	this->b = b;

	fadedFrame = std::make_shared<cv::Mat>(video->GetFrame()->size(), CV_8UC3, cv::Scalar(b, g, r));
	currentFrame = std::make_shared<cv::Mat>();

	end = false;
}

FadeAnimationVideo::~FadeAnimationVideo()
{
}

std::shared_ptr<cv::Mat> FadeAnimationVideo::GetFrame()
{
	if (end)
		return fadedFrame;

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime >= duration) {
		elapsedTime = duration;
		end = true;
	}

	std::shared_ptr<cv::Mat> sourceframe = video->GetFrame();

	float weight = elapsedTime / duration;
	cv::addWeighted(*sourceframe, 1.0f - weight, *fadedFrame, weight, 0.0, *currentFrame);

	return currentFrame;
}

#endif // !_FADE_ANIMATION_H
