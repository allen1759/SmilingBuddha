/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _WAVE_TRANSITIONAL_VIDEO_H
#define _WAVE_TRANSITIONAL_VIDEO_H

#include "TransitionalVideo.h"

class WaveTransitionalVideo : public TransitionalVideo
{
public:
	WaveTransitionalVideo(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration);
	
	virtual ~WaveTransitionalVideo() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

private:
	std::shared_ptr<cv::Mat> currentFrame;

	bool end;

};

#endif // !_WAVE_TRANSITIONAL_VIDEO_H
