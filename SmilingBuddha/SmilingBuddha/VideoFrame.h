/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_FRAME_H
#define _VIDEO_FRAME_H

#include "Video.h"

#include <memory>

#include <opencv2\core\core.hpp>

class VideoFrame : public Video, public std::enable_shared_from_this<VideoFrame>
{
public:
	VideoFrame(std::shared_ptr<cv::Mat> image);

	virtual ~VideoFrame() override;

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual std::shared_ptr<Video> GetVideo() override;

private:
	std::shared_ptr<cv::Mat> frame;
};

#endif // !_VIDEO_FRAME_H
