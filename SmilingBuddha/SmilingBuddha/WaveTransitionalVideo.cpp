#include "WaveTransitionalVideo.h"


#include <opencv2\imgproc\imgproc.hpp>
WaveTransitionalVideo::WaveTransitionalVideo(std::shared_ptr<Video> video, std::shared_ptr<Video> nextVideo, float duration)
	: TransitionalVideo(video, nextVideo, duration)
{
	currentFrame = std::make_shared<cv::Mat>();

	end = false;
}

WaveTransitionalVideo::~WaveTransitionalVideo()
{
}

std::shared_ptr<cv::Mat> WaveTransitionalVideo::GetFrame()
{
	if (end)
		return nextVideo->GetFrame();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime >= duration) {
		elapsedTime = duration;
		end = true;
	}

	std::shared_ptr<cv::Mat> fromFrame = video->GetFrame();
	std::shared_ptr<cv::Mat> toFrame = nextVideo->GetFrame();

	float weight = elapsedTime / duration;
	cv::addWeighted(*fromFrame, 1.0f - weight, *toFrame, weight, 0.0, *currentFrame);

	cv::rectangle(*currentFrame, cv::Point(0, 0), cv::Point(currentFrame->cols, currentFrame->rows), cv::Scalar(0, 0, 255), 5);

	return currentFrame;
}
