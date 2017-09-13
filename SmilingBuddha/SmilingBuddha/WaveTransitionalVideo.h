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
	//TODO: make constant.
	float PI_2 = 1.57079632679489662f;
	int DOWNSAMPLE_FACTOR = 4;
	double DAMPING_FACTOR = 0.001f;
	float WAVE_AMPLITUDE = 16.0f;

	std::shared_ptr<cv::Mat> heightMap;
	std::shared_ptr<cv::Mat> currentHeightMap;
	std::shared_ptr<cv::Mat> velocityMap;
	
	std::shared_ptr<cv::Mat> currentFrame;

	float damping;
	
	std::chrono::high_resolution_clock::time_point lastTime;
	float secondsPerFrame;

	bool end;

};

#endif // !_WAVE_TRANSITIONAL_VIDEO_H
