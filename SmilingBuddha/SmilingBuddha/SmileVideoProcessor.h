/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_VIDEO_PROCESSOR_H
#define _SMILE_VIDEO_PROCESSOR_H

#include <thread>
#include <memory>

#include <dlib/image_processing.h>

#include "SmileProcessStrategy.h"
#include "SmileRecognizer.h"
#include "SmileSaver.h"

class SmileVideoProcessor
{
public:
	void SetSmileProcessStrategy(SmileProcessStrategy *smileProcessStrategy);

	void Start();

	void Stop();

protected:
	SmileVideoProcessor();

	virtual ~SmileVideoProcessor();
	
	virtual void ProcessSmileVideo() = 0;

	void ProcessFrame();

	virtual std::shared_ptr<cv::Mat> ReadFrame() = 0;
	
	std::shared_ptr<cv::Mat> CaptureFace(const cv::Mat &originFrame, double &currentIntensity);

	cv::Rect CalculateSmileVideoRect(const cv::Point &leftEyePosition, const cv::Point &rightEyePosition) const;


	bool isRunning;
	std::shared_ptr<std::thread> processSmileVideoThread;
	
	cv::Rect webcamROI;
	cv::CascadeClassifier faceClassifier;
	dlib::shape_predictor eyePredictor;

	SmileProcessStrategy *smileProcessStrategy;

	// For recognize intensity of smile.
	SmileRecognizer *recognizer;
	double lastIntensity;
	cv::Point lastLeftEyePosition;
	cv::Point lastRightEyePosition;

	SmileSaver *saver;
};

#endif // !_SMILE_VIDEO_PROCESSOR_H 