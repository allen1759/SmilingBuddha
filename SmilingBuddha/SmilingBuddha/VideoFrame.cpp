#include "VideoFrame.h"

VideoFrame::VideoFrame(std::shared_ptr<cv::Mat> image)
{
	this->frame = image;
}

VideoFrame::~VideoFrame()
{
}

std::shared_ptr<cv::Mat> VideoFrame::GetFrame()
{
	return frame;
}

std::shared_ptr<Video> VideoFrame::GetVideo()
{
	return shared_from_this();
}
