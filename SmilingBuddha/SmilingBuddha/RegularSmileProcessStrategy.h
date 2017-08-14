/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _REGULAR_SMILE_PROCESS_STRATEGY_H
#define _REGULAR_SMILE_PROCESS_STRATEGY_H

#include "SmileProcessStrategy.h"
#include <memory>
#include <vector>
#include <opencv2/core.hpp>

class RegularSmileProcessStrategy : public SmileProcessStrategy
{
public:
	RegularSmileProcessStrategy(SmileObserver * observer, int imageSequenceLength);
	~RegularSmileProcessStrategy();

	virtual void InitializeProcess() override;

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;

private:

	const double SMILE_INTENSITY_THRESHOLD = 0.0;
	const int IMAGE_SEQUENCE_LENGTH = 40;
	bool isRecord = false;

	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageBuffer = NULL;
	//std::vector<double> intensityBuffer;
};

#endif // !_REGULAR_SMILE_PROCESS_STRATEGY_H
