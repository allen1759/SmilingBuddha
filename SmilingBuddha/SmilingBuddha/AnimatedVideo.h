/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ANIMATED_VIDEO_H
#define _ANIMATED_VIDEO_H

#include "Video.h"

#include <iostream>

// TODO: this is abstract class

class AnimatedVideo : public Video
{
public:
	AnimatedVideo(Video * video)
	{
		this->video = video->GetVideo();
	}
	virtual ~AnimatedVideo()
	{}

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual Video * GetVideo() override;

protected:

	Video * video;
};


std::shared_ptr<cv::Mat> AnimatedVideo::GetFrame()
{
	video->GetFrame();
	std::cout << "Animated Video" << std::endl;
	return NULL;
}
Video * AnimatedVideo::GetVideo()
{
	return video;
}
#endif // !_ANIMATED_VIDEO_H
