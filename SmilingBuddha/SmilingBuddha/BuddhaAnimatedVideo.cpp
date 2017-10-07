/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "BuddhaAnimatedVideo.h"

#include <cmath>

#include <opencv2\imgproc\imgproc.hpp>

#include <opencv2\imgcodecs\imgcodecs.hpp>

BuddhaAnimatedVideo::BuddhaAnimatedVideo(std::shared_ptr<Video> video, float duration, int centerX, int centerY)
	: AnimatedVideo(video, duration)
{
	buddhaFrame = std::make_shared<cv::Mat>(cv::imread("resources\\buddha_animation_image.png", -1));
	
	std::shared_ptr<cv::Mat> frame = video->GetFrame();
	
	this->centerX = centerX;
	this->centerY = centerY;

	radius = frame->rows > frame->cols ? frame->rows : frame->cols;
	buddhaRadius = buddhaFrame->rows > buddhaFrame->cols ? buddhaFrame->rows / 2 : buddhaFrame->cols / 2;
	
	currentFrame = std::make_shared<cv::Mat>(frame->rows, frame->cols, CV_8UC3);
	blendFrame = std::make_shared<cv::Mat>(frame->rows, frame->cols, CV_8UC3);

	end = false;
	lastTime = startTime;
}

BuddhaAnimatedVideo::~BuddhaAnimatedVideo()
{
}

std::shared_ptr<cv::Mat> BuddhaAnimatedVideo::GetFrame()
{
	if (end)
		return video->GetFrame();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();
	if (elapsedTime >= duration) {
		elapsedTime = duration;
		end = true;
	}
	float weight = elapsedTime / duration;

	std::shared_ptr<cv::Mat> frame = video->GetFrame();


	#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int i = 0; i < currentFrame->rows; ++i) {
		for (int j = 0; j < currentFrame->cols; ++j) {
			blendFrame->at<cv::Vec3b>(i, j) = frame->at<cv::Vec3b>(i, j);

			float currentRadius = weight * radius;
			float diameter = currentRadius * 2.0f;

			int rowIndex = static_cast<int>((static_cast<float>(i - centerY) + currentRadius) * static_cast<float>(buddhaFrame->rows) / diameter);
			int colIndex = static_cast<int>((static_cast<float>(j - centerX) + currentRadius) * static_cast<float>(buddhaFrame->cols) / diameter);
			
			if (rowIndex >= 0 && rowIndex < buddhaFrame->rows - 1 && colIndex >= 0 && colIndex < buddhaFrame->cols - 1) {
				if (buddhaFrame->at<cv::Vec4b>(rowIndex, colIndex)[3]) {
					float offsetRow = rowIndex - buddhaFrame->rows / 2;
					float offsetCol = colIndex - buddhaFrame->cols / 2;

					float alpha = std::max(1.0f - std::pow(std::sqrt(offsetRow * offsetRow + offsetCol * offsetCol) / buddhaRadius, 5.0f) - weight, 0.0f);

					blendFrame->at<cv::Vec3b>(i, j)[0] = buddhaFrame->at<cv::Vec4b>(rowIndex, colIndex)[0] * alpha + blendFrame->at<cv::Vec3b>(i, j)[0] * (1.0f - alpha);
					blendFrame->at<cv::Vec3b>(i, j)[1] = buddhaFrame->at<cv::Vec4b>(rowIndex, colIndex)[1] * alpha + blendFrame->at<cv::Vec3b>(i, j)[1] * (1.0f - alpha);
					blendFrame->at<cv::Vec3b>(i, j)[2] = buddhaFrame->at<cv::Vec4b>(rowIndex, colIndex)[2] * alpha + blendFrame->at<cv::Vec3b>(i, j)[2] * (1.0f - alpha);
				}
				
			}
		}
	}
	
	#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int i = 0; i < currentFrame->rows; ++i) {
		for (int j = 0; j < currentFrame->cols; ++j) {
			float offsetX = j - centerX;
			float offsetY = i - centerY;

			float dist = std::sqrt(offsetX*offsetX + offsetY*offsetY) / radius;

			float sinArg = (dist - weight) * PERIOD;
			float height = (WAVE_AMPLITUDE / dist) * std::min(std::sin(sinArg) / sinArg, 1.0f) * (1.0f - weight);

			int rowIndex = std::min(std::max(i + static_cast<int>(offsetY * height + 0.5f), 0), currentFrame->rows - 1);
			int colIndex = std::min(std::max(j + static_cast<int>(offsetX * height + 0.5f), 0), currentFrame->cols - 1);
			currentFrame->at<cv::Vec3b>(i, j)[0] = blendFrame->at<cv::Vec3b>(rowIndex, colIndex)[0];
			currentFrame->at<cv::Vec3b>(i, j)[1] = blendFrame->at<cv::Vec3b>(rowIndex, colIndex)[1];
			currentFrame->at<cv::Vec3b>(i, j)[2] = blendFrame->at<cv::Vec3b>(rowIndex, colIndex)[2];
		}
	}
	
	return currentFrame;
}
