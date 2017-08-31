/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _TRANSITIONAL_VIDEO_H
#define _TRANSITIONAL_VIDEO_H

#include "Video.h"

#include <chrono>

#include <iostream>

class TransitionalVideo : public Video
{
public:
	TransitionalVideo(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration)
	{
		this->video = video->GetVideo();
		this->nextVideo = nextVideo->GetVideo();

		this->duration = std::chrono::milliseconds(static_cast<int>(duration * 1000));
		startTime = std::chrono::high_resolution_clock::now();
	}
	virtual ~TransitionalVideo()
	{}

	virtual std::shared_ptr<cv::Mat> GetFrame() = 0;

	virtual std::shared_ptr<Video> GetVideo() override
	{
		return nextVideo;
	}

protected:

	std::shared_ptr<Video> video;
	std::shared_ptr<Video> nextVideo;

	std::chrono::milliseconds duration;

	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_TRANSITIONAL_VIDEO_H
