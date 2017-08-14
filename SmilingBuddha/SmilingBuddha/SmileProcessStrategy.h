/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_PROCESS_STRATEGY_H
#define _SMILE_PROCESS_STRATEGY_H

#include <memory>

#include <opencv2/core.hpp>
#include "SmileObserver.h"

class SmileProcessStrategy
{
public:
	virtual void InitializeProcess() = 0;
	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) = 0;

protected:
	SmileObserver * observer;

private:

};

#endif // !_SMILE_PROCESS_STRATEGY_H
