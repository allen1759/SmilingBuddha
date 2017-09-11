
#include "FadeAnimationVideo.h"

FadeAnimationVideo::FadeAnimationVideo(std::shared_ptr<Video> video, float duration, unsigned char r, unsigned char g, unsigned char b)
	: AnimatedVideo(video, duration)
{
	this->r = r;
	this->g = g;
	this->b = b;

	fadedFrame = std::make_shared<cv::Mat>(video->GetFrame()->size(), CV_8UC3, cv::Scalar(b, g, r));
	currentFrame = std::make_shared<cv::Mat>();

	end = false;
}

FadeAnimationVideo::~FadeAnimationVideo()
{
}

std::shared_ptr<cv::Mat> FadeAnimationVideo::GetFrame()
{
	if (end)
		return fadedFrame;

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime >= duration) {
		elapsedTime = duration;
		end = true;
	}

	std::shared_ptr<cv::Mat> sourceframe = video->GetFrame();

	float weight = elapsedTime / duration;
	cv::addWeighted(*sourceframe, 1.0f - weight, *fadedFrame, weight, 0.0, *currentFrame);

	return currentFrame;
}
