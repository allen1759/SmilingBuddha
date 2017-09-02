/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ANIMATED_VIDEO_H
#define _ANIMATED_VIDEO_H

#include "Video.h"

#include <chrono>

#include <iostream>

// TODO: this is abstract class

class AnimatedVideo : public Video
{
public:
	AnimatedVideo(std::shared_ptr<Video> video, float duration)
	{
		this->video = video->GetVideo();

		this->duration = std::chrono::milliseconds(static_cast<int>(duration * 1000));
		this->startTime = std::chrono::high_resolution_clock::now();
	}
	virtual ~AnimatedVideo()
	{}

	virtual std::shared_ptr<cv::Mat> GetFrame() = 0;

	virtual std::shared_ptr<Video> GetVideo() override;

protected:

	std::shared_ptr<Video> video;

	std::chrono::milliseconds duration;
	std::chrono::high_resolution_clock::time_point startTime;
};

std::shared_ptr<Video> AnimatedVideo::GetVideo()
{
	return video;
}

#endif // !_ANIMATED_VIDEO_H
