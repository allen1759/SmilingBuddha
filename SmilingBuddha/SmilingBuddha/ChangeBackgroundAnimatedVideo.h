/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _CHANGE_BACKGROUND_ANIMATED_VIDEO_H
#define _CHANGE_BACKGROUND_ANIMATED_VIDEO_H

#include "AnimatedVideo.h"

#include <opencv2\core\core.hpp>

class ChangeBackgroundAnimatedVideo : public AnimatedVideo
{
public:
	ChangeBackgroundAnimatedVideo(std::shared_ptr<Video> video, float duration);

	virtual ~ChangeBackgroundAnimatedVideo() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;
private:
	std::shared_ptr<cv::Mat> mask;
	std::shared_ptr<cv::Mat> currentBlurMask;
	std::shared_ptr<cv::Mat> dilateElement;

	std::shared_ptr<cv::Mat> currentFrame;

	std::chrono::high_resolution_clock::time_point lastTime;
	float secondsPerFrame;
	
	bool end;
};
#endif // !_CHANGE_BACKGROUND_ANIMATED_VIDEO_H
