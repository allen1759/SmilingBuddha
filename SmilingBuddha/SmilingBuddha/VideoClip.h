/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_CLIP_H
#define _VIDEO_CLIP_H

#include "Video.h"

#include <vector>

class VideoClip : public Video
{
public:
	VideoClip(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence);

	virtual ~VideoClip() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual Video * GetVideo() override;

private:
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence;
};


#endif // !_VIDEO_CLIP_H

