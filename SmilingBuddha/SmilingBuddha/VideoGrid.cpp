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
{
	rowCount = Setting::GetInstance()->GetRow();
	colCount = Setting::GetInstance()->GetCol();

	videoGrid.resize(rowCount * colCount);
	layout.resize(rowCount * colCount);
	//TODO: read from setting.
	float gridSizeX = 1920.0f / static_cast<float>(colCount);
	float gridSizeY = 1080.0f / static_cast<float>(rowCount);
	for (int i = 0; i < rowCount; ++i) {
		for (int j = 0; j < colCount; ++j)
			layout[i * colCount + j] = cv::Point(static_cast<int>(gridSizeX * static_cast<float>(j)), static_cast<int>(gridSizeY * static_cast<float>(i)));
	}

	frame = std::make_shared<cv::Mat>(1080, 1920, CV_8UC3);
}

VideoGrid::~VideoGrid()
{
}

std::shared_ptr<cv::Mat> VideoGrid::GetFrame()
{
	for (int i = 0; i < rowCount; ++i) {
		for (int j = 0; j < colCount; ++j)
			DrawSingleGrid(i, j);
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
