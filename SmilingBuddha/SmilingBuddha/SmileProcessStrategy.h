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
#include "SmileSaver.h"

class SmileProcessStrategy
{
public:
	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) = 0;

	virtual ~SmileProcessStrategy()
	{}

protected:
	SmileObserver *observer;
	SmileSaver *saver;

private:

};

#endif // !_SMILE_PROCESS_STRATEGY_H
