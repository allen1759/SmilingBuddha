/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _BLENDING_TRANSITION_H
#define _BLENDING_TRANSITION_H

#include "TransitionalVideo.h"

class BlendingTransitionVideo : public TransitionalVideo
{
public:
	BlendingTransitionVideo(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration);

	virtual ~BlendingTransitionVideo() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

private:

	std::shared_ptr<cv::Mat> firstFrame;
	std::shared_ptr<cv::Mat> lastFrame;
};



#endif // !_BLENDING_TRANSITION_H


