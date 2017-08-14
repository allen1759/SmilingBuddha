/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _WEBCAM_SMILE_VIDEO_PROCESSOR_H
#define _WEBCAM_SMILE_VIDEO_PROCESSOR_H

#include "SmileVideoProcessor.h"

#include <opencv2/videoio/videoio.hpp>

class WebcamSmileVideoProcessor : public SmileVideoProcessor
{
public:
	WebcamSmileVideoProcessor();

	virtual ~WebcamSmileVideoProcessor() override;

	static WebcamSmileVideoProcessor * GetInstance();

private:
	static WebcamSmileVideoProcessor *instance;

	virtual void ProcessSmileVideo() override;

	virtual std::shared_ptr<cv::Mat> ReadFrame() override;

	void InitializeWebcam();

	
	// Index of the video capturing device to open. If a single camera connected, just set to 0.
	const int CAMERA_INDEX = 0;

	cv::VideoCapture capture;
};

#endif // !_WEBCAM_SMILE_VIDEO_PROCESSOR_H
