/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "ChangeBackgroundAnimatedVideo.h"

#include <memory>

ChangeBackgroundAnimatedVideo::ChangeBackgroundAnimatedVideo(std::shared_ptr<Video> video, float duration)
	:AnimatedVideo(video, duration)
{
}

ChangeBackgroundAnimatedVideo::~ChangeBackgroundAnimatedVideo()
{
}

std::shared_ptr<cv::Mat> ChangeBackgroundAnimatedVideo::GetFrame()
{
	return std::make_shared<cv::Mat>(video->GetFrame()->size(), CV_8UC3, cv::Scalar(0, 0, 255));
}
