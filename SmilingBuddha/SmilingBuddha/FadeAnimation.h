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
	FadeAnimation(std::shared_ptr<Video> video, float duration);
	~FadeAnimation();

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

private:

};

FadeAnimation::FadeAnimation(std::shared_ptr<Video> video, float duration) : AnimatedVideo(video, duration)
{
}

FadeAnimation::~FadeAnimation()
{
}

std::shared_ptr<cv::Mat> FadeAnimation::GetFrame()
{
	std::shared_ptr<cv::Mat> src1 = video->GetFrame();
	std::shared_ptr<cv::Mat> src2 = std::make_shared<cv::Mat>(cv::Mat(src1->rows, src1->cols, CV_8UC3, cv::Scalar(255, 255, 255)));
	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	double delta = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count());
	double beta = delta / static_cast<double>(duration.count());

	cv::addWeighted(*src1, 1-beta, *src2, beta, 0.0, *dst);

	return dst;
}

#endif // !_FADE_ANIMATION_H
