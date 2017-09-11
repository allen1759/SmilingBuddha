/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _CHANGE_BACKGROUND_ANIMATED_VIDEO_H
#define _CHANGE_BACKGROUND_ANIMATED_VIDEO_H

#include "AnimatedVideo.h"

class ChangeBackgroundAnimatedVideo : public AnimatedVideo
{
public:
	ChangeBackgroundAnimatedVideo(std::shared_ptr<Video> video, float duration);

	virtual ~ChangeBackgroundAnimatedVideo() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;
private:

};
#endif // !_CHANGE_BACKGROUND_ANIMATED_VIDEO_H
