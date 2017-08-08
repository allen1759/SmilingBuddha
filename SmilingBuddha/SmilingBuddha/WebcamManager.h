/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _WEBCAM_MANAGER_H
#define _WEBCAM_MANAGER_H

#include <thread>
#include <memory>

#include <dlib/image_processing.h>

#include "SmileProcess.h"
#include "SmileRecognizer.h"
#include "SmileSaver.h"

class WebcamManager
{
public:
	static WebcamManager * GetInstance();

	~WebcamManager();

	void SetSmileProcess(SmileProcess *smileProcess);

	void Start();

	void Stop();

private:
	WebcamManager();

	void ProcessingFrame();
	
	void InitializeWebcam();

	// Index of the video capturing device to open. If a single camera connected, just set to 0.
	const int CAMERA_INDEX = 3;

	bool isRunning;
	std::shared_ptr<std::thread> processFrameThread;

	cv::Rect webcamROI;
	cv::CascadeClassifier faceClassifier;
	dlib::shape_predictor eyePredictor;


	static WebcamManager *instance;

	SmileProcess *smileProcess;

	// For recognize intensity of smile.
	SmileRecognizer *recognizer;
	double lastIntensity;
	cv::Point lastLeftEyePosition;
	cv::Point lastRightEyePosition;

	SmileSaver *saver;
};

#endif // !_WEBCAM_MANAGER_H 