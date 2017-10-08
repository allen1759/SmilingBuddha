/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _DEBUG_SMILE_VIDEO_PROCESSOR_H
#define _DEBUG_SMILE_VIDEO_PROCESSOR_H

#include "SmileVideoProcessor.h"

#include <memory>
#include <conio.h>

#include <opencv2/highgui/highgui.hpp>

#include "InputManager.h"

class DebugSmileVideoProcessor : public SmileVideoProcessor
{
public:
	DebugSmileVideoProcessor();

	virtual ~DebugSmileVideoProcessor();

	static DebugSmileVideoProcessor * GetInstance();

private:

	virtual void ProcessSmileVideo() override;

	virtual std::shared_ptr<cv::Mat> ReadFrame() override;

	void ChangeSmile();


	static DebugSmileVideoProcessor *instance;

	bool isSmile;
	std::shared_ptr<cv::Mat> smileFrame;
	std::shared_ptr<cv::Mat> unsmileFrame;
};

DebugSmileVideoProcessor * DebugSmileVideoProcessor::instance = NULL;

DebugSmileVideoProcessor::DebugSmileVideoProcessor() : SmileVideoProcessor()
{
	isSmile = false;
	smileFrame = std::make_shared<cv::Mat>(cv::imread("resources\\smile1920.jpg"));
	unsmileFrame = std::make_shared<cv::Mat>(cv::imread("resources\\unsmile1920.jpg"));
}

DebugSmileVideoProcessor::~DebugSmileVideoProcessor()
{
}

DebugSmileVideoProcessor * DebugSmileVideoProcessor::GetInstance()
{
	if (instance == NULL)
		instance = new DebugSmileVideoProcessor();

	return instance;
}

void DebugSmileVideoProcessor::ChangeSmile()
{
	isSmile = !isSmile;
}

void DebugSmileVideoProcessor::ProcessSmileVideo()
{
	while (isRunning) {
		ProcessFrame();
	}
}

std::shared_ptr<cv::Mat> DebugSmileVideoProcessor::ReadFrame()
{
	if (InputManager::GetInstance()->GetKey('x'))
		ChangeSmile();

	if (isSmile)
		return std::make_shared<cv::Mat>(*smileFrame);
	else
		return std::make_shared<cv::Mat>(*unsmileFrame);
}

#endif // !_DEBUG_SMILE_VIDEO_PROCESSOR_H
