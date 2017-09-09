/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "EndingBuddhaState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "BlendingTransitionVideo.h"
#include "EndingFadeState.h"

EndingBuddhaState::EndingBuddhaState(Director *director)
	: EndingState(director)
{
	this->buddhaVideo = std::make_shared<VideoClip>(EndingState::videoPool->GetNextBuddhaVideo(), BUDDHA_VIDEO_TIME, false, false);
	this->currentDistance = 0;

	this->nextAppearElapsedTime = 0.0f;
}

EndingBuddhaState::~EndingBuddhaState()
{
}

void EndingBuddhaState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > BUDDHA_STATE_TIME) {
		director->SetInteractionState(std::make_shared<EndingFadeState>(director));
		return;
	}
	else if (elapsedTime > nextAppearElapsedTime) {
		EndingState::AppearAnimation(currentDistance);
		currentDistance++;
		nextAppearElapsedTime += APEAR_TIME_FOR_EACH;
	}
}

std::string EndingBuddhaState::ToString()
{
	return "EndingBuddhaState";
}

void EndingBuddhaState::SetTransition(int row, int col)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		buddhaVideo,
		WAVE_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}
