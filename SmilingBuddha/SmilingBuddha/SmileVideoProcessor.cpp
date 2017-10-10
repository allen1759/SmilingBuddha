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

#include "Setting.h"

//#include <iostream>
//#include <chrono>

SmileVideoProcessor::SmileVideoProcessor()
	: IMAGE_WIDTH(Setting::GetInstance()->GetImageWidth()),
	  IMAGE_HEIGHT(Setting::GetInstance()->GetImageHeight())
{
	isRunning = false;
	processSmileVideoThread = NULL;

	// TODO: read from file.
	detectROI = cv::Rect(0, 0, 1920, 1080);
	if (!faceClassifier.load("resources\\haarcascade_frontalface_default.xml"))
		throw std::runtime_error("Cascade file not found.");
	dlib::deserialize("resources\\shape_predictor_68_face_landmarks.dat") >> eyePredictor;

	lastIntensity = 0;
	lastLeftEyePosition = cv::Point(0, 0);
	lastRightEyePosition = cv::Point(0, 0);

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

void SmileVideoProcessor::SetSmileProcessStrategy(std::shared_ptr<SmileProcessStrategy> smileProcessStrategy)
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
		//std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		std::shared_ptr<cv::Mat> frame = ReadFrame();
		if (frame == NULL)
			break;

		std::shared_ptr<cv::Mat> smileFrame = CaptureFace(*frame);

		if (smileProcessStrategy)
			smileProcessStrategy->ProcessSmile(smileFrame, lastIntensity);

		//std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		//std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		//std::cout << duration.count() << std::endl;
	}
}

std::shared_ptr<cv::Mat> SmileVideoProcessor::CaptureFace(const cv::Mat &originFrame)
{
	double currentIntensity = 0.0;
	cv::Point currentLeftEyePosition;
	cv::Point currentRightEyePosition;

	cv::Mat roiFrame = originFrame(detectROI);
	cv::Mat grayFrame;
	cv::cvtColor(roiFrame, grayFrame, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(grayFrame, grayFrame);

	std::shared_ptr<cv::Rect> faceRect = DetectFace(grayFrame);
	if (faceRect != NULL) {
		cv::Mat faceFrame = grayFrame(*faceRect);
		DetectEyes(faceFrame, currentLeftEyePosition, currentRightEyePosition);

		currentLeftEyePosition += detectROI.tl() + faceRect->tl();
		currentRightEyePosition += detectROI.tl() + faceRect->tl();

		
		float weight = GetSmoothWeight(currentLeftEyePosition - lastLeftEyePosition, currentRightEyePosition - lastRightEyePosition);
		currentLeftEyePosition = (1.0f - weight) * lastLeftEyePosition + weight * currentLeftEyePosition;
		currentRightEyePosition = (1.0f - weight) * lastRightEyePosition + weight * currentRightEyePosition;
		currentIntensity = (lastIntensity + SmileRecognizer::GetInstance()->Recognize(faceFrame)) * 0.5;
	}
	else {
		currentLeftEyePosition = lastLeftEyePosition;
		currentRightEyePosition = lastRightEyePosition;
		currentIntensity = 0.0;
	}

	lastLeftEyePosition = currentLeftEyePosition;
	lastRightEyePosition = currentRightEyePosition;
	lastIntensity = currentIntensity;

	return CropSmileFrame(originFrame, currentLeftEyePosition, currentRightEyePosition);
}

std::shared_ptr<cv::Rect> SmileVideoProcessor::DetectFace(const cv::Mat & frame)
{
	std::vector<cv::Rect> faceList;
	faceClassifier.detectMultiScale(frame, faceList, 1.5, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(100, 100));

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
		return std::make_shared<cv::Rect>(faceList[largestFaceIndex]);
	}

	return NULL;
}

void SmileVideoProcessor::DetectEyes(const cv::Mat & faceFrame, cv::Point & left, cv::Point & right)
{
	// Use dlib landmark to detect position of eyes.
	dlib::array2d<unsigned char> dlibFaceFrame;
	dlib::assign_image(dlibFaceFrame, dlib::cv_image<unsigned char>(faceFrame));
	dlib::full_object_detection landmarkResult = eyePredictor(dlibFaceFrame, dlib::rectangle(0, 0, dlibFaceFrame.nc(), dlibFaceFrame.nr()));
	dlib::point eyeLandmarks[4] = { landmarkResult.part(36), landmarkResult.part(39), landmarkResult.part(42), landmarkResult.part(45) };

	// Use lanmark #36, #39 to calculate lefteye.
	left = cv::Point((eyeLandmarks[0].x() + eyeLandmarks[1].x()) >> 1,
					(eyeLandmarks[0].y() + eyeLandmarks[1].y()) >> 1);
	// Use lanmark #42, #45 to calculate lefteye.
	right = cv::Point((eyeLandmarks[2].x() + eyeLandmarks[3].x()) >> 1,
					(eyeLandmarks[2].y() + eyeLandmarks[3].y()) >> 1);
}

std::shared_ptr<cv::Mat> SmileVideoProcessor::CropSmileFrame(const cv::Mat &originFrame, const cv::Point &leftEyePosition, const cv::Point &rightEyePosition) const
{
	float eyeDistance = static_cast<float>(rightEyePosition.x - leftEyePosition.x);
	int width = static_cast<int>(3.8f * eyeDistance);
	int height = static_cast<int>(5.3f * eyeDistance);

	cv::Point eyeCenter = (leftEyePosition + rightEyePosition) / 2;

	width = std::min(originFrame.cols - 1, width);
	height = std::min(originFrame.rows - 1, height);
	
	int x = eyeCenter.x - (width >> 1);
	int y = eyeCenter.y - static_cast<int>(height * 0.42307692f);


	// Check the edges of Rect.
	if (x < 0)
		x = 0;
	if (x + width >= originFrame.cols)
		x = originFrame.cols - width - 1;
	if (y < 0)
		y = 0;
	if (y + height >= originFrame.rows)
		y = originFrame.rows - height - 1;

	std::shared_ptr<cv::Mat> smileFrame = std::make_shared<cv::Mat>();
	cv::resize(originFrame(cv::Rect(x, y, width, height)), *smileFrame, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	return smileFrame;
}

float SmileVideoProcessor::GetSmoothWeight(const cv::Point & leftEyeShift, const cv::Point & rightEyeShift) const
{
	const int CLAMP_SHIFT_LOWER_THRESHOLD = 30;
	const int CLAMP_SHIFT_UPPER_THRESHOLD = 300;

	int totalShiftPixel = std::abs(leftEyeShift.x) + std::abs(leftEyeShift.y) + std::abs(rightEyeShift.x) + std::abs(rightEyeShift.y);
	totalShiftPixel = std::min(CLAMP_SHIFT_UPPER_THRESHOLD, std::max(0, totalShiftPixel - CLAMP_SHIFT_LOWER_THRESHOLD));
	
	return totalShiftPixel / static_cast<float>(CLAMP_SHIFT_UPPER_THRESHOLD*3);
}


