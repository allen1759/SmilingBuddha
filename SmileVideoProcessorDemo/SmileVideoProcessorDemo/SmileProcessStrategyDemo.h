/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_PROCESS_DEMO_STRATEGY_H
#define _SMILE_PROCESS_DEMO_STRATEGY_H

#include <iostream>
#include <memory>
#include <mutex>

#include <opencv2/highgui/highgui.hpp>

#include "../../SmilingBuddha/SmilingBuddha/SmileProcessStrategy.h"

class SmileProcessStrategyDemo : public SmileProcessStrategy
{
public:
	SmileProcessStrategyDemo(SmileObserver *observer);
	~SmileProcessStrategyDemo();

	virtual void InitializeProcess() override;

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;

	std::shared_ptr<cv::Mat> img;

private:

	std::mutex mutex;
};

SmileProcessStrategyDemo::SmileProcessStrategyDemo(SmileObserver * observer)
{
	this->observer = observer;
}

SmileProcessStrategyDemo::~SmileProcessStrategyDemo()
{
}

void SmileProcessStrategyDemo::InitializeProcess()
{
}

void SmileProcessStrategyDemo::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
	this->img = img;

	std::cout << "intensity: " << intensity << std::endl;
}

#endif // !_SMILE_PROCESS_DEMO_STRATEGY_H
