/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "WebcamManager.h"

#include <stdexcept>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <dlib/opencv.h>


#include <iostream>
#include <chrono>

WebcamManager * WebcamManager::instance = NULL;

WebcamManager::WebcamManager()
{
	isRunning = false;
	processFrameThread = NULL;

	// TODO: read from text file
	// TODO: put last eye postion in the middle of ROI
	webcamROI = cv::Rect(480, 0, 1440, 1080);
	lastLeftEyePosition = cv::Point(910, 540);
	lastRightEyePosition = cv::Point(1010, 540);

	if (!faceClassifier.load("resources\\haarcascade_frontalface_default.xml"))
		throw std::runtime_error("Cascade file not found.");
	dlib::deserialize("resources\\shape_predictor_68_face_landmarks.dat") >> eyePredictor;

	// TODO: singleton
	recognizer = new SmileRecognizer();
}

WebcamManager * WebcamManager::GetInstance()
{
	if (instance == NULL)
		instance = new WebcamManager();

	return instance;
}

WebcamManager::~WebcamManager()
{
	Stop();
	if (processFrameThread) {
		processFrameThread->join();
		processFrameThread = NULL;
	}
}

void WebcamManager::SetSmileProcess(SmileProcessStrategy *smileProcess)
{
	this->smileProcessStrategy = smileProcess;
}

void WebcamManager::Start()
{
	if (!isRunning) {
		if (processFrameThread) {
			processFrameThread->join();
			processFrameThread = NULL;
		}
		isRunning = true;
		processFrameThread = std::make_shared<std::thread>(&WebcamManager::ProcessingFrame, this);
	}
}

void WebcamManager::Stop()
{
	if (isRunning)
		isRunning = false;
}

void WebcamManager::ProcessingFrame()
{
	cv::VideoCapture capture;

	while (isRunning) {
		capture.open(CAMERA_INDEX);
		capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
		capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

		while (isRunning) {
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

			cv::Mat frame;
			if (!capture.read(frame))
				break;
			
			cv::Mat grayFrame;
			cv::cvtColor(frame(webcamROI), grayFrame, cv::COLOR_BGR2GRAY);
			cv::equalizeHist(grayFrame, grayFrame);

			std::vector<cv::Rect> faceList;
			faceClassifier.detectMultiScale(grayFrame, faceList, 1.5, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(100, 100));
			std::cout << "face size: " << faceList.size() << std::endl;
			cv::Point currentLeftEyePosition;
			cv::Point currentRightEyePosition;
			double currentIntensity;

			if (faceList.size() != 0) {
				int largestFaceIndex = 0;
				int largestFaceArea = 0;
				for (int i = 0; i < faceList.size(); ++i) {
					int faceArea = faceList[i].area();
					if (faceArea > largestFaceArea) {
						largestFaceIndex = i;
						largestFaceArea = faceArea;
					}
					//cv::Point center(faceList[i].x + faceList[i].width / 2, faceList[i].y + faceList[i].height / 2);
					//cv:x:ellipse(frame, center, cv::Size(faceList[i].width / 2, faceList[i].height / 2), 0, 0, 360, cv::Scalar(0, 255, 0), 2, 8, 0);
				}
				cv::Rect &faceRect = faceList[largestFaceIndex];
				cv::Mat faceFrame = grayFrame(faceRect);

				dlib::array2d<unsigned char> dlibFaceFrame;
				dlib::assign_image(dlibFaceFrame, dlib::cv_image<unsigned char>(faceFrame));
				
				dlib::full_object_detection landmarkResult = eyePredictor(dlibFaceFrame, dlib::rectangle(0, 0, dlibFaceFrame.nc(), dlibFaceFrame.nr()));
				dlib::point eyeLandmarks[4] = { landmarkResult.part(36), landmarkResult.part(39), landmarkResult.part(42), landmarkResult.part(45) };
				//cv::circle(frame, cv::Point(face.x + pt.x(), face.y + pt.y()), 3, cv::Scalar(255, 0, 0), 5);

				cv::Point leftEyePosition = cv::Point((eyeLandmarks[0].x() + eyeLandmarks[1].x()) >> 1,
													  (eyeLandmarks[0].y() + eyeLandmarks[1].y()) >> 1);
				leftEyePosition += cv::Point(faceRect.x, faceRect.y) + webcamROI.tl();
				cv::Point rightEyePosition = cv::Point((eyeLandmarks[2].x() + eyeLandmarks[3].x()) >> 1,
													   (eyeLandmarks[2].y() + eyeLandmarks[3].y()) >> 1);
				rightEyePosition += cv::Point(faceRect.x, faceRect.y) + webcamROI.tl();

				currentLeftEyePosition = (lastLeftEyePosition + leftEyePosition) / 2;
				currentRightEyePosition = (lastRightEyePosition + rightEyePosition) / 2;
				currentIntensity = (lastIntensity + recognizer->Recognize(faceFrame)) * 0.5;
			}
			else {
				currentLeftEyePosition = lastLeftEyePosition;
				currentRightEyePosition = lastRightEyePosition;

				currentIntensity = 0.0;
			}

			float eyeDistance = static_cast<float>(currentRightEyePosition.x - currentLeftEyePosition.x);
			int eyeHeight = (currentLeftEyePosition.y + currentRightEyePosition.y) / 2;
			int x = currentLeftEyePosition.x - static_cast<int>(1.4f * eyeDistance);
			int y = eyeHeight - static_cast<int>(2.2f * eyeDistance);
			int width = static_cast<int>(3.8f * eyeDistance);
			int height = static_cast<int>(5.3f * eyeDistance);
			if (x < 0)
				x = 0;
			if (x + width >= 1920)
				x = 1920 - width - 1;
			if (y < 0)
				y = 0;
			if (y + height >= 1080)
				y = 1080 - height - 1;

			cv::Rect smileRect = cv::Rect(x, y, width, height);

			std::shared_ptr<cv::Mat> smileFrame = std::make_shared<cv::Mat>(frame(smileRect));

			if (smileProcessStrategy)
				smileProcessStrategy->ProcessSmile(smileFrame, currentIntensity);

			lastIntensity = currentIntensity;
			lastLeftEyePosition = currentLeftEyePosition;
			lastRightEyePosition = currentRightEyePosition;
			
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			std::cout << duration.count() << std::endl;
		}
	}
}

void WebcamManager::InitializeWebcam()
{
}

