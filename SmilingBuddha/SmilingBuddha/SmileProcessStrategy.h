/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_PROCESS_STRATEGY_H
#define _SMILE_PROCESS_STRATEGY_H

#include <vector>
#include <memory>

#include <opencv2/core.hpp>
#include "SmileObserver.h"
#include "SmileSaver.h"

class SmileProcessStrategy
{
public:
	SmileProcessStrategy(SmileObserver *observer);

	virtual ~SmileProcessStrategy();

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) = 0;


protected:
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> ResizeImageSequence(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs);

	SmileObserver *observer;
	SmileSaver *saver;

	const double SMILE_INTENSITY_THRESHOLD = 0.5;
	int IMAGE_SEQUENCE_LENGTH;
	const int IMAGE_WIDTH;
	const int IMAGE_HEIGHT;
	const int SAVE_IMAGE_WIDTH;
	const int SAVE_IMAGE_HEIGHT;

private:

};

#endif // !_SMILE_PROCESS_STRATEGY_H
