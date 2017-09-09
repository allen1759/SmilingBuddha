/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "BuddhaState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "BlendingTransitionVideo.h"



#include "IntroInitialState.h"

BuddhaState::BuddhaState(Director *director)
	: EndingState(director),
	  APEAR_TIME_FOR_EACH(APEAR_TIME / Setting::GetInstance()->GetMaxDistanceToCenter())
{
	this->buddhaVideo = std::make_shared<VideoClip>(EndingState::videoPool->GetNextBuddhaVideo(), BUDDHA_VIDEO_TIME, false, false);
	this->currentDistance = 0;

	this->nextAppearElapsedTime = 0.0f;
	
}

BuddhaState::~BuddhaState()
{
}

void BuddhaState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > BUDDHA_STATE_TIME) {
		director->SetInteractionState(std::make_shared<IntroInitialState>(director));
		return;
	}
	else if (elapsedTime > nextAppearElapsedTime) {
		EndingState::AppearAnimation(currentDistance);
		currentDistance++;
		nextAppearElapsedTime += APEAR_TIME_FOR_EACH;
	}
}

std::string BuddhaState::ToString()
{
	return "BuddhaState";
}

void BuddhaState::SetTransition(int row, int col)
{
	SetBlendingVideo(row, col, buddhaVideo);
}

void BuddhaState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}
