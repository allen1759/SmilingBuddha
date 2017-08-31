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
	RegularSmileProcessStrategy(SmileObserver * observer);

	~RegularSmileProcessStrategy();

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;

private:

	bool isRecord;
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageBuffer;

};

#endif // !_REGULAR_SMILE_PROCESS_STRATEGY_H
