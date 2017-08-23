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

class VideoClip : public Video
{
public:
	VideoClip(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence, float duration);

	virtual ~VideoClip() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual std::shared_ptr<Video> GetVideo() override;

private:
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence;

	std::chrono::milliseconds duration;

	std::chrono::high_resolution_clock::time_point startTime;
};


#endif // !_VIDEO_CLIP_H

