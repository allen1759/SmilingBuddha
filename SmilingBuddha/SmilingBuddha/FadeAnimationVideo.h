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

#endif // !_FADE_ANIMATION_H
