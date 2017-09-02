/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoGrid.h"

#include "Setting.h"

VideoGrid::VideoGrid()
{
	this->rowCount = Setting::GetInstance()->GetRow();
	this->colCount = Setting::GetInstance()->GetCol();

	videoGrid.resize(rowCount * colCount);
}

VideoGrid::~VideoGrid()
{
}

std::shared_ptr<cv::Mat> VideoGrid::GetFrame()
{
	return std::shared_ptr<cv::Mat>();
}

std::shared_ptr<Video> VideoGrid::GetVideo()
{
	return std::make_shared<VideoGrid>(*this);
}

std::shared_ptr<Video> VideoGrid::GetChild(int row, int col)
{
	return videoGrid[row * colCount + col];
}

void VideoGrid::SetChild(std::shared_ptr<Video> video, int row, int col)
{
	videoGrid[row * colCount + col] = video;
}
