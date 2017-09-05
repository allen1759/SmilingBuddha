/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "BlendingTransitionVideo.h"

BlendingTransitionVideo::BlendingTransitionVideo(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration)
	: TransitionalVideo(video, nextVideo, duration)
{
	firstFrame = video->GetFrame();
	lastFrame = nextVideo->GetFrame();
}

BlendingTransitionVideo::~BlendingTransitionVideo()
{
}

std::shared_ptr<cv::Mat> BlendingTransitionVideo::GetFrame()
{
	std::shared_ptr<cv::Mat> dst;

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	double delta = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count());

	if (delta > duration.count()) {
		dst = nextVideo->GetFrame();
	}
	else {
		dst = std::make_shared<cv::Mat>();
		lastFrame = nextVideo->GetFrame();
		double weight = delta / static_cast<double>(duration.count());
		cv::addWeighted(*firstFrame, 1 - weight, *lastFrame, weight, 0.0, *dst);
	}

	return dst;
}

