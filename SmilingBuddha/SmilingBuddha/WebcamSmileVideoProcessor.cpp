/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "WebcamSmileVideoProcessor.h"


WebcamSmileVideoProcessor *WebcamSmileVideoProcessor::instance = NULL;

WebcamSmileVideoProcessor::WebcamSmileVideoProcessor() : SmileVideoProcessor()
{
	// TODO: read from text file
	// TODO: put last eye postion in the middle of ROI
	detectROI = cv::Rect(480, 0, 1440, 1080);

	lastLeftEyePosition = cv::Point(910, 540);
	lastRightEyePosition = cv::Point(1010, 540);

	InitializeWebcam();
}

WebcamSmileVideoProcessor::~WebcamSmileVideoProcessor()
{
}

WebcamSmileVideoProcessor * WebcamSmileVideoProcessor::GetInstance()
{
	if (instance == NULL)
		instance = new WebcamSmileVideoProcessor();

	return instance;
}

void WebcamSmileVideoProcessor::ProcessSmileVideo()
{
	while (isRunning) {
		capture.open(CAMERA_INDEX);
		capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
		capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

		ProcessFrame();

		capture.release();
	}
}

std::shared_ptr<cv::Mat> WebcamSmileVideoProcessor::ReadFrame()
{
	std::shared_ptr<cv::Mat> frame = std::make_shared<cv::Mat>();
	if (capture.read(*frame))
		return frame;
	else
		return NULL;
}

void WebcamSmileVideoProcessor::InitializeWebcam()
{
	// TODO
}
