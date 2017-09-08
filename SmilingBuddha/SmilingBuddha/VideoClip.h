/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_CLIP_H
#define _VIDEO_CLIP_H

#include "Video.h"

#include <vector>
#include <chrono>
#include <memory>

class VideoClip : public Video, public std::enable_shared_from_this<VideoClip>
{
public:
	VideoClip(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence, float videoTime, bool loop, bool reverse);

	virtual ~VideoClip() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual std::shared_ptr<Video> GetVideo() override;

private:
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence;
	int currentFrameIndex;

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point lastTime;
	float elapsedTime;

	float framePerSecond;
	float secondsPerFrame;

	bool loop;
	bool reverse;
	bool end;
};


#endif // !_VIDEO_CLIP_H

