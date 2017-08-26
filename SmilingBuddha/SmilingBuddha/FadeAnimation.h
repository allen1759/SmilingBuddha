/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/


#ifndef _FADE_ANIMATION_H
#define _FADE_ANIMATION_H

#include "AnimatedVideo.h"


#include <opencv2/highgui/highgui.hpp>

class FadeAnimation : public AnimatedVideo
{
public:
	FadeAnimation(std::shared_ptr<Video> video, float duration, unsigned char R, unsigned char G, unsigned char B);
	
	virtual ~FadeAnimation() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

private:
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

FadeAnimation::FadeAnimation(std::shared_ptr<Video> video, float duration, unsigned char R, unsigned char G, unsigned char B)
	: AnimatedVideo(video, duration)
{
	this->R = R;
	this->G = G;
	this->B = B;
}

FadeAnimation::~FadeAnimation()
{
}

std::shared_ptr<cv::Mat> FadeAnimation::GetFrame()
{
	std::shared_ptr<cv::Mat> src1 = video->GetFrame();
	std::shared_ptr<cv::Mat> src2 = std::make_shared<cv::Mat>(cv::Mat(src1->rows, src1->cols, CV_8UC3, cv::Scalar(B, G, R)));
	std::shared_ptr<cv::Mat> dst;

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	double delta = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count());

	if (delta > duration.count())
		dst = src2;
	else {
		dst = std::make_shared<cv::Mat>();
		double weight = delta / static_cast<double>(duration.count());
		cv::addWeighted(*src1, 1 - weight, *src2, weight, 0.0, *dst);
	}

	return dst;
}

#endif // !_FADE_ANIMATION_H
