/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "ChangeBackgroundAnimatedVideo.h"

#include <memory>

#include <opencv2\imgproc\imgproc.hpp>

ChangeBackgroundAnimatedVideo::ChangeBackgroundAnimatedVideo(std::shared_ptr<Video> video, float duration)
	:AnimatedVideo(video, duration)
{
	std::shared_ptr<cv::Mat> frame = video->GetFrame();

	std::shared_ptr<cv::Mat> tempMask = std::make_shared<cv::Mat>(frame->rows + 2, frame->cols + 2, CV_8U, 0);

	cv::floodFill(*frame, *tempMask, cv::Point(0, 0), 255, 0, cv::Scalar(), cv::Scalar(), 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY);
	cv::floodFill(*frame, *tempMask, cv::Point(0, frame->rows / 2), 255, 0, cv::Scalar(), cv::Scalar(), 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY);
	cv::floodFill(*frame, *tempMask, cv::Point(frame->cols - 1, 0), 255, 0, cv::Scalar(), cv::Scalar(), 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY);
	cv::floodFill(*frame, *tempMask, cv::Point(frame->cols - 1, frame->rows / 2), 255, 0, cv::Scalar(), cv::Scalar(), 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY);

	mask = std::make_shared<cv::Mat>(frame->rows, frame->cols, CV_8U);
	(*tempMask)(cv::Rect(1, 1, frame->cols, frame->rows)).copyTo(*mask);
	
	currentBlurMask = std::make_shared<cv::Mat>(frame->rows, frame->cols, CV_8U);
	cv::GaussianBlur(*mask, *currentBlurMask, cv::Size(25, 25), 10, 10);

	currentFrame = std::make_shared<cv::Mat>(frame->rows, frame->cols, CV_8UC3);
	
	int totalStep = frame->cols < frame->rows ? frame->cols / 4 : frame->rows / 4;
	float totalFrameCount = std::min(totalStep, static_cast<int>(duration * 30.0f));
	int elementSize = (totalStep / totalFrameCount) * 2 + 1;
	secondsPerFrame = duration / totalFrameCount;

	dilateElement = std::make_shared<cv::Mat>(elementSize, elementSize, CV_8U, 1);

	end = false;;
	lastTime = startTime;
}

ChangeBackgroundAnimatedVideo::~ChangeBackgroundAnimatedVideo()
{
}

std::shared_ptr<cv::Mat> ChangeBackgroundAnimatedVideo::GetFrame()
{
	if (end)
		return video->GetFrame();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastTime).count();

	if (deltaTime > secondsPerFrame) {
		lastTime = currentTime;
		cv::erode(*mask, *mask, *dilateElement);
		cv::GaussianBlur(*mask, *currentBlurMask, cv::Size(25, 25), 10, 10);
	}

	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();
	if (elapsedTime >= duration) {
		elapsedTime = duration;
		end = true;
	}

	std::shared_ptr<cv::Mat> frame = video->GetFrame();

	#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int i = 0; i < currentFrame->rows; ++i) {
		for (int j = 0; j < currentFrame->cols; ++j) {
			if (currentBlurMask->at<uchar>(i, j) != 0) {
				currentFrame->at<cv::Vec3b>(i, j)[0] = 255 - currentBlurMask->at<uchar>(i, j);
				currentFrame->at<cv::Vec3b>(i, j)[1] = 255 - currentBlurMask->at<uchar>(i, j);
				currentFrame->at<cv::Vec3b>(i, j)[2] = 255 - currentBlurMask->at<uchar>(i, j);
			}
			else
				currentFrame->at<cv::Vec3b>(i, j) = frame->at<cv::Vec3b>(i, j);
		}
	}
	
	return currentFrame;
}
