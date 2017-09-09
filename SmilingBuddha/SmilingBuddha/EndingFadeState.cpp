/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "EndingFadeState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "FadeAnimationVideo.h"



#include "IntroInitialState.h"

EndingFadeState::EndingFadeState(Director *director)
	: EndingState(director)
{
	std::shared_ptr<cv::Mat> white = std::make_shared<cv::Mat>(
		cv::Size(Setting::GetInstance()->GetImageWidth(), Setting::GetInstance()->GetImageHeight()),
		CV_8UC3,
		cv::Scalar(255, 255, 255));
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> whiteVideo = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
	whiteVideo->push_back(white);
	endingFadeVideo = std::make_shared<VideoClip>(whiteVideo, FADE_VIDEO_TIME, false, false);

	this->currentDistance = 0;
	this->nextAppearElapsedTime = 0.0f;
}

EndingFadeState::~EndingFadeState()
{
}

void EndingFadeState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > ENDING_FADE_STATE_TIME) {
		director->SetInteractionState(std::make_shared<IntroInitialState>(director));
		return;
	}
	else if (elapsedTime > nextAppearElapsedTime) {
		EndingState::AppearAnimation(currentDistance);
		currentDistance++;
		nextAppearElapsedTime += APEAR_TIME_FOR_EACH;
	}
}

std::string EndingFadeState::ToString()
{
	return "EndingFadeState";
}

void EndingFadeState::SetTransition(int row, int col)
{
	// Set white Fade Animation Video.
	std::shared_ptr<Video> fadeVideo = std::make_shared<FadeAnimationVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		FADE_ANIMATION_TIME, 255, 255, 255);

	director->GetVideoGrid()->SetChild(fadeVideo, row, col);
}
