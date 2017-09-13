#include "WaveTransitionalVideo.h"

#include <cmath>

#include <opencv2\imgproc\imgproc.hpp>

WaveTransitionalVideo::WaveTransitionalVideo(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration)
	: TransitionalVideo(video, nextVideo, duration)
{
	std::shared_ptr<cv::Mat> frame = video->GetFrame();
	int heightMapWidth = frame->cols / DOWNSAMPLE_FACTOR;
	int heightMapHeight = frame->rows / DOWNSAMPLE_FACTOR;

	int radius;
	float totalFrameCount;
	if (heightMapWidth > heightMapHeight) {
		totalFrameCount = heightMapWidth << 1;
		radius = heightMapWidth / 16;
	}
	else {
		totalFrameCount = heightMapHeight << 1;
		radius = heightMapHeight / 16;
	}

	damping = static_cast<float>(std::pow(DAMPING_FACTOR, 1.0 / static_cast<double>(totalFrameCount)));

	heightMap = std::make_shared<cv::Mat>(heightMapHeight, heightMapWidth, CV_32F, cv::Scalar::all(0.0f));

	int centerRow = heightMapHeight / 2;
	int centerCol = heightMapWidth / 2;
	for (int i = -radius; i <= radius; ++i) {
		int i2 = i * i;
		for (int j = -radius; j <= radius; ++j) {
			float dist = std::sqrt(static_cast<float>(i2 + j * j));
			if (dist <= radius) {
				heightMap->at<float>(centerRow + i, centerCol + j) = WAVE_AMPLITUDE *std::cos(dist * PI_2 / static_cast<float>(radius));
			}
		}
	}

	velocityMap = std::make_shared<cv::Mat>(heightMapHeight, heightMapWidth, CV_32F, cv::Scalar::all(0.0f));
	
	currentFrame = std::make_shared<cv::Mat>(frame->rows, frame->cols, CV_8UC3);
	currentHeightMap = std::make_shared<cv::Mat>();
	cv::resize(*heightMap, *currentHeightMap, cv::Size(frame->cols, frame->rows), cv::INTER_LINEAR);

	secondsPerFrame = duration / totalFrameCount;
	end = false;
	lastTime = startTime;
}

WaveTransitionalVideo::~WaveTransitionalVideo()
{
}

std::shared_ptr<cv::Mat> WaveTransitionalVideo::GetFrame()
{
	if (end)
		return nextVideo->GetFrame();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastTime).count();
	
	if (deltaTime > secondsPerFrame) {
		lastTime = currentTime;
		#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
		for (int i = 1; i < velocityMap->rows - 1; ++i) {
			for (int j = 1; j < velocityMap->cols - 1; ++j) {
				velocityMap->at<float>(i, j) += ((heightMap->at<float>(i + 1, j) + heightMap->at<float>(i - 1, j) +
					heightMap->at<float>(i, j + 1) + heightMap->at<float>(i, j - 1)) * 0.25f - heightMap->at<float>(i, j));

				velocityMap->at<float>(i, j) *= damping;
			}
		}

		#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
		for (int i = 0; i < heightMap->rows; ++i) {
			for (int j = 0; j < velocityMap->cols; ++j)
				heightMap->at<float>(i, j) += velocityMap->at<float>(i, j);
		}

		cv::resize(*heightMap, *currentHeightMap, cv::Size(currentHeightMap->cols, currentHeightMap->rows), cv::INTER_LINEAR);
	}

	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();
	if (elapsedTime >= duration) {
		elapsedTime = duration;
		end = true;
	}

	float weight = elapsedTime / duration;

	int centerRow = currentFrame->rows >> 1;
	int centerCol = currentFrame->cols >> 1;

	std::shared_ptr<cv::Mat> fromFrame = video->GetFrame();
	std::shared_ptr<cv::Mat> toFrame = nextVideo->GetFrame();

	#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int i = 0; i < currentFrame->rows; ++i) {
		for (int j = 0; j < currentFrame->cols; ++j) {
			float height = currentHeightMap->at<float>(i, j);

			int rowIndex = std::min(std::max(i + static_cast<int>(static_cast<float>(i - centerRow) * height), 0), currentFrame->rows - 1);
			int colIndex = std::min(std::max(j + static_cast<int>(static_cast<float>(j - centerCol) * height), 0), currentFrame->cols - 1);
			currentFrame->at<cv::Vec3b>(i, j)[0] = static_cast<float>(fromFrame->at<cv::Vec3b>(rowIndex, colIndex)[0]) * (1.0 - weight) + 
				static_cast<float>(toFrame->at<cv::Vec3b>(rowIndex, colIndex)[0]) * weight;
			currentFrame->at<cv::Vec3b>(i, j)[1] = static_cast<float>(fromFrame->at<cv::Vec3b>(rowIndex, colIndex)[1]) * (1.0 - weight) +
				static_cast<float>(toFrame->at<cv::Vec3b>(rowIndex, colIndex)[1]) * weight;
			currentFrame->at<cv::Vec3b>(i, j)[2] = static_cast<float>(fromFrame->at<cv::Vec3b>(rowIndex, colIndex)[2]) * (1.0 - weight) +
				static_cast<float>(toFrame->at<cv::Vec3b>(rowIndex, colIndex)[2]) * weight;
		}
	}

	return currentFrame;
}
