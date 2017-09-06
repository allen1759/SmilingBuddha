/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoGrid.h"

//#include <opencv2/core/core.hpp>
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
	std::shared_ptr<cv::Mat> combine = std::make_shared<cv::Mat>();
	for (int i = 0; i < rowCount; ++i) {
		std::shared_ptr<cv::Mat> row = std::make_shared<cv::Mat>();
		for (int j = 0; j < colCount; ++j) {
			std::shared_ptr<cv::Mat> img;
			videoGridMutex.lock();
			if (!videoGrid[i * colCount + j]) {
				img = std::make_shared<cv::Mat>(177, 130, CV_8UC3, cv::Scalar(0, 0, 0));
			}
			else {
				img = videoGrid[i * colCount + j]->GetFrame();
			}
			videoGridMutex.unlock();

			if (row->empty())
				img->copyTo(*row);
			else
				cv::hconcat(*row, *img, *row);
		}

		if (combine->empty())
			row->copyTo(*combine);
		else
			cv::vconcat(*combine, *row, *combine);
	}

	return combine;
}

std::shared_ptr<Video> VideoGrid::GetVideo()
{
	//return std::make_shared<VideoGrid>(*this);
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
