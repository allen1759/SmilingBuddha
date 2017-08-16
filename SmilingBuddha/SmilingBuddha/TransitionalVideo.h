/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _TRANSITIONAL_VIDEO_H
#define _TRANSITIONAL_VIDEO_H

#include "Video.h"

#include <iostream>

class TransitionalVideo : public Video
{
public:
	TransitionalVideo(Video * video, Video * nextVideo)
	{
		this->video = video->GetVideo();
		this->nextVideo = nextVideo->GetVideo();
	}
	~TransitionalVideo()
	{}

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual Video * GetVideo() override
	{
		return nextVideo;
	}

protected:

	Video * video;
	Video * nextVideo;
};


std::shared_ptr<cv::Mat> TransitionalVideo::GetFrame()
{
	video->GetFrame();
	nextVideo->GetFrame();
	std::cout << "Transitional Video" << std::endl;
	return NULL;
}

#endif // !_TRANSITIONAL_VIDEO_H
