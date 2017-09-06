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
protected:
	SmileVideoProcessor();

	virtual ~SmileVideoProcessor();
	
public:
	void SetSmileProcessStrategy(std::shared_ptr<SmileProcessStrategy> smileProcessStrategy);

	void Start();

	void Stop();

protected:
	virtual void ProcessSmileVideo() = 0;

	void ProcessFrame();

	virtual std::shared_ptr<cv::Mat> ReadFrame() = 0;
	
	std::shared_ptr<cv::Mat> CaptureFace(const cv::Mat &originFrame);

	std::shared_ptr<cv::Rect> DetectFace(const cv::Mat &frame);

	void DetectEyes(const cv::Mat &faceFrame, cv::Point &left, cv::Point &right);

	std::shared_ptr<cv::Mat> CropSmileFrame(const cv::Mat &originFrame, const cv::Point &leftEyePosition, const cv::Point &rightEyePosition) const;

	float GetSmoothWeight(const cv::Point &leftEyeShift, const cv::Point &rightEyeShift) const;


	bool isRunning;
	std::shared_ptr<std::thread> processSmileVideoThread;
	
	cv::Rect detectROI;
	cv::CascadeClassifier faceClassifier;
	dlib::shape_predictor eyePredictor;
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;

	std::shared_ptr<SmileProcessStrategy> smileProcessStrategy;

	// For recognize intensity of smile.
	SmileRecognizer *recognizer;
	double lastIntensity;
	cv::Point lastLeftEyePosition;
	cv::Point lastRightEyePosition;

	SmileSaver *saver;
};

#endif // !_SMILE_VIDEO_PROCESSOR_H 