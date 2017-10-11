/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _BUDDHA_ANIMATED_VIDEO_H
#define _BUDDHA_ANIMATED_VIDEO_H

#include "AnimatedVideo.h"

class BuddhaAnimatedVideo : public AnimatedVideo
{
public:
	BuddhaAnimatedVideo(std::shared_ptr<Video> video, float duration, int centerX, int centerY);

	virtual ~BuddhaAnimatedVideo() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

private:
	const float PERIOD = 3.14159f * 80.0f;
	
	float WAVE_AMPLITUDE = 0.025f;

	float radius;
	float buddhaRadius;

	std::shared_ptr<cv::Mat> blendFrame;
	std::shared_ptr<cv::Mat> currentFrame;
	std::shared_ptr<cv::Mat> buddhaFrame;

	int centerX;
	int centerY;

	std::chrono::high_resolution_clock::time_point lastTime;
	float secondsPerFrame;

	bool end;

};

#endif // !_BUDDHA_ANIMATED_VIDEO_H
