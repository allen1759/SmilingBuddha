/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoGrid.h"

VideoGrid::VideoGrid(int rowCount, int colCount)
{
	videoGrid = new Video *[rowCount * colCount];

	this->rowCount = rowCount;
	this->colCount = colCount;
}

VideoGrid::~VideoGrid()
{
}

std::shared_ptr<cv::Mat> VideoGrid::GetFrame()
{
	return std::shared_ptr<cv::Mat>();
}

Video * VideoGrid::GetVideo()
{
	return this;
}

Video * VideoGrid::GetChild(int row, int col)
{
	return nullptr;
}

void VideoGrid::SetChild(Video * video, int row, int col)
{
}
