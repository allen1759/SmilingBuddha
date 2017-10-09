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
	//layout.resize(rowCount * colCount);

	//float gridSizeX = Setting::GetInstance()->GetResolutionWidth() / static_cast<float>(colCount);
	//float gridSizeY = Setting::GetInstance()->GetResolutionHeight() / static_cast<float>(rowCount);
	//for (int i = 0; i < rowCount; ++i) {
	//	for (int j = 0; j < colCount; ++j)
	//		layout[i * colCount + j] = cv::Point(static_cast<int>(gridSizeX * static_cast<float>(j)), static_cast<int>(gridSizeY * static_cast<float>(i)));
	//}

	
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
/*
void VideoGrid::DrawSingleGrid(int row, int col)
{
	std::shared_ptr<cv::Mat> gridFrame;
	int gridIndex = row * colCount + col;

	videoGridMutex.lock();
	if (videoGrid[gridIndex])
		gridFrame = videoGrid[gridIndex]->GetFrame();
	videoGridMutex.unlock();

	if (!gridFrame)
		return;

	cv::Point gridLayout = layout[gridIndex];

	#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int i = 0; i < gridFrame->rows; ++i) {
		int frameIndex = ((gridLayout.y + i) * frame->cols + gridLayout.x) * 3;
		int gridFrameIndex = i * gridFrame->cols * 3;
		for (int j = 0; j < gridFrame->cols; ++j) {
			frame->data[frameIndex++] = gridFrame->data[gridFrameIndex++];
			frame->data[frameIndex++] = gridFrame->data[gridFrameIndex++];
			frame->data[frameIndex++] = gridFrame->data[gridFrameIndex++];
		}
	}
}
*/