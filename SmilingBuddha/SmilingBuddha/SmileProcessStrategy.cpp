/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SmileProcessStrategy.h"

#include <opencv2\imgproc\imgproc.hpp>

#include "Setting.h"

SmileProcessStrategy::SmileProcessStrategy(SmileObserver * observer)
	: IMAGE_SEQUENCE_LENGTH(Setting::GetInstance()->GetImageSequenceLength()),
	  IMAGE_WIDTH(Setting::GetInstance()->GetImageWidth()),
	  IMAGE_HEIGHT(Setting::GetInstance()->GetImageHeight()),
	  SAVE_IMAGE_WIDTH(Setting::GetInstance()->GetSaveImageWidth()),
	  SAVE_IMAGE_HEIGHT(Setting::GetInstance()->GetSaveIMageHeight())
{
	this->observer = observer;
	this->saver = SmileSaver::GetInstance();
}

SmileProcessStrategy::~SmileProcessStrategy()
{
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> SmileProcessStrategy::ResizeImageSequence(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs)
{
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> ret = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

	for (int i = 0; i < imgs->size(); ++i) {
		std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
		cv::resize(*imgs->at(i), *dst, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
		ret->push_back(dst);
	}
	
	return ret;
}

