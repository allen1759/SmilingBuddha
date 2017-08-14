/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SmileVideoProcessor.h"

#include <stdexcept>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <dlib/opencv.h>


#include <iostream>
#include <chrono>

SmileVideoProcessor::SmileVideoProcessor()
{
	isRunning = false;
	processSmileVideoThread = NULL;

	webcamROI = cv::Rect(0, 0, 1920, 1080);
	if (!faceClassifier.load("resources\\haarcascade_frontalface_default.xml"))
		throw std::runtime_error("Cascade file not found.");
	dlib::deserialize("resources\\shape_predictor_68_face_landmarks.dat") >> eyePredictor;

	// TODO: singleton
	recognizer = new SmileRecognizer();

	lastIntensity = 0;
	lastLeftEyePosition = cv::Point(910, 540);
	lastRightEyePosition = cv::Point(1010, 540);

	saver = SmileSaver::GetInstance();
}

SmileVideoProcessor::~SmileVideoProcessor()
{
	Stop();
	if (processSmileVideoThread) {
		processSmileVideoThread->join();
		processSmileVideoThread = NULL;
	}
}

void SmileVideoProcessor::SetSmileProcessStrategy(SmileProcessStrategy *smileProcessStrategy)
{
	this->smileProcessStrategy = smileProcessStrategy;
}

void SmileVideoProcessor::Start()
{
	if (!isRunning) {
		if (processSmileVideoThread) {
			processSmileVideoThread->join();
			processSmileVideoThread = NULL;
		}
		isRunning = true;
		processSmileVideoThread = std::make_shared<std::thread>(&SmileVideoProcessor::ProcessSmileVideo, this);
	}
}

void SmileVideoProcessor::Stop()
{
	if (isRunning)
		isRunning = false;
}

void SmileVideoProcessor::ProcessFrame()
{
	while (isRunning) {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		std::shared_ptr<cv::Mat> frame = ReadFrame();
		if (frame == NULL)
			break;

		double currentIntensity;
		std::shared_ptr<cv::Mat> smileFrame = CaptureFace(*frame, currentIntensity);

		if (smileProcessStrategy)
			smileProcessStrategy->ProcessSmile(smileFrame, currentIntensity);

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << duration.count() << std::endl;
	}
}

std::shared_ptr<cv::Mat> SmileVideoProcessor::CaptureFace(const cv::Mat &originFrame, double &currentIntensity)
{
	cv::Point currentLeftEyePosition;
	cv::Point currentRightEyePosition;

	cv::Mat grayFrame;
	cv::cvtColor(originFrame(webcamROI), grayFrame, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(grayFrame, grayFrame);

	std::vector<cv::Rect> faceList;
	faceClassifier.detectMultiScale(grayFrame, faceList, 1.5, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(100, 100));
	//std::cout << "face size: " << faceList.size() << std::endl;

	// If faceClassifier can detect any face in frame.
	if (faceList.size() != 0) {
		int largestFaceIndex = 0;
		int largestFaceArea = 0;
		// Find the face with largest area.
		for (int i = 0; i < faceList.size(); ++i) {
			int faceArea = faceList[i].area();
			if (faceArea > largestFaceArea) {
				largestFaceIndex = i;
				largestFaceArea = faceArea;
			}
		}
		cv::Rect &faceRect = faceList[largestFaceIndex];
		cv::Mat faceFrame = grayFrame(faceRect);

		// Use dlib landmark to detect position of eyes.
		dlib::array2d<unsigned char> dlibFaceFrame;
		dlib::assign_image(dlibFaceFrame, dlib::cv_image<unsigned char>(faceFrame));
		dlib::full_object_detection landmarkResult = eyePredictor(dlibFaceFrame, dlib::rectangle(0, 0, dlibFaceFrame.nc(), dlibFaceFrame.nr()));
		dlib::point eyeLandmarks[4] = { landmarkResult.part(36), landmarkResult.part(39), landmarkResult.part(42), landmarkResult.part(45) };

		// Use lanmark #36, #39 to calculate lefteye.
		cv::Point leftEyePosition = cv::Point((eyeLandmarks[0].x() + eyeLandmarks[1].x()) >> 1,
											  (eyeLandmarks[0].y() + eyeLandmarks[1].y()) >> 1);
		leftEyePosition += cv::Point(faceRect.x, faceRect.y) + webcamROI.tl();
		// Use lanmark #42, #45 to calculate lefteye.
		cv::Point rightEyePosition = cv::Point((eyeLandmarks[2].x() + eyeLandmarks[3].x()) >> 1,
			                                   (eyeLandmarks[2].y() + eyeLandmarks[3].y()) >> 1);
		rightEyePosition += cv::Point(faceRect.x, faceRect.y) + webcamROI.tl();

		currentLeftEyePosition = (lastLeftEyePosition + leftEyePosition) / 2;
		currentRightEyePosition = (lastRightEyePosition + rightEyePosition) / 2;
		currentIntensity = (lastIntensity + recognizer->Recognize(faceFrame)) * 0.5;
	}
	// If faceClassifier cannot detect any face in frame.
	else {
		currentLeftEyePosition = lastLeftEyePosition;
		currentRightEyePosition = lastRightEyePosition;
		currentIntensity = 0.0;
	}

	lastLeftEyePosition = currentLeftEyePosition;
	lastRightEyePosition = currentRightEyePosition;
	lastIntensity = currentIntensity;

	cv::Rect smileRect(CalculateSmileVideoRect(currentLeftEyePosition, currentRightEyePosition));
	return std::make_shared<cv::Mat>(originFrame(smileRect));
}

cv::Rect SmileVideoProcessor::CalculateSmileVideoRect(const cv::Point &leftEyePosition, const cv::Point &rightEyePosition) const
{
	float eyeDistance = static_cast<float>(rightEyePosition.x - leftEyePosition.x);
	int eyeHeight = (leftEyePosition.y + rightEyePosition.y) / 2;
	
	int x = leftEyePosition.x - static_cast<int>(1.4f * eyeDistance);
	int y = eyeHeight - static_cast<int>(2.2f * eyeDistance);
	int width = static_cast<int>(3.8f * eyeDistance);
	int height = static_cast<int>(5.3f * eyeDistance);

	// Check the edges of Rect.
	if (x < 0)
		x = 0;
	if (x + width >= 1920)
		x = 1920 - width - 1;
	if (y < 0)
		y = 0;
	if (y + height >= 1080)
		y = 1080 - height - 1;

	return cv::Rect(x, y, width, height);
}


