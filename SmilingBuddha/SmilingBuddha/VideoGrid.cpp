/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoGrid.h"

//#include <opencv2/core/core.hpp>
#include "Setting.h"

#include <algorithm>

VideoGrid::VideoGrid()
	: layout(Setting::GetInstance()->GetLayout())
{
	rowCount = Setting::GetInstance()->GetRow();
	colCount = Setting::GetInstance()->GetCol();

	videoGrid.resize(rowCount * colCount);
}

VideoGrid::~VideoGrid()
{
}

std::shared_ptr<cv::Mat> VideoGrid::GetFrame()
{
	int resolutionWidth = Setting::GetInstance()->GetResolutionWidth();
	int resolutionHeight = Setting::GetInstance()->GetResolutionHeight();

	std::shared_ptr<cv::Mat> frame = std::make_shared<cv::Mat>(resolutionHeight, resolutionWidth, CV_8UC3);
	for (int i = 0; i < videoGrid.size(); ++i) {
		std::shared_ptr<cv::Mat> gridFrame;
		videoGridMutex.lock();
		if (videoGrid[i])
			gridFrame = videoGrid[i]->GetFrame();
		videoGridMutex.unlock();

		if (gridFrame)
			gridFrame->copyTo((*frame)(cv::Rect(layout[i].x, layout[i].y, gridFrame->cols, gridFrame->rows)));
	}
	return frame;
}

std::shared_ptr<Video> VideoGrid::GetVideo()
{
	return shared_from_this();
}

std::shared_ptr<Video> VideoGrid::GetChild(int row, int col)
{
	return videoGrid[row * colCount + col];
}

void VideoGrid::SetChild(std::shared_ptr<Video> video, int row, int col)
{
	videoGridMutex.lock();
	videoGrid[row * colCount + col] = video;
	videoGridMutex.unlock();
}
