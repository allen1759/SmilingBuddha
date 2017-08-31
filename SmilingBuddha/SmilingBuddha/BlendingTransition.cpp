/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "BlendingTransition.h"

BlendingTransition::BlendingTransition(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration)
	: TransitionalVideo(video, nextVideo, duration)
{
	blendingIndex = 0;
	firstFrame = video->GetFrame();
	lastFrame = nextVideo->GetFrame();
}

BlendingTransition::~BlendingTransition()
{
}

std::shared_ptr<cv::Mat> BlendingTransition::GetFrame()
{
	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();

	double weight = static_cast<double>(blendingIndex) / (BLENDING_SEQUENCE_SIZE - 1);
	cv::addWeighted(*firstFrame, (1 - weight), *lastFrame, weight, 0.0, *dst);
	blendingIndex++;

	return dst;
}