/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_PROCESS_DEMO_H
#define _SMILE_PROCESS_DEMO_H

#include <iostream>
#include <memory>

#include <opencv2/highgui/highgui.hpp>

#include "../../SmilingBuddha/SmilingBuddha/SmileProcess.h"

class SmileProcessDemo : public SmileProcess
{
public:
	SmileProcessDemo(SmileObserver *observer);
	~SmileProcessDemo();

	virtual void InitializeProcess() override;

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;

	std::shared_ptr<cv::Mat> img;

private:


};

SmileProcessDemo::SmileProcessDemo(SmileObserver * observer)
{
	this->observer = observer;
}

SmileProcessDemo::~SmileProcessDemo()
{
}

void SmileProcessDemo::InitializeProcess()
{
}

void SmileProcessDemo::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
	this->img = img;
	std::cout << intensity << std::endl;
}
#endif // !_SMILE_PROCESS_DEMO_H
