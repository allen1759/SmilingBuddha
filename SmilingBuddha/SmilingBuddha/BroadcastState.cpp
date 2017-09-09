/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "BroadcastState.h"

#include <algorithm>

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "EndingBuddhaState.h"


#include "BlendingTransitionVideo.h"
#include "IntroInitialState.h"

BroadcastState::BroadcastState(Director *director)
	: InteractionState(director),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	  BROADCAST_TIME_FOR_EACH(BROADCAST_TIME / Setting::GetInstance()->GetWindowCount())
{
	int windowCount = Setting::GetInstance()->GetWindowCount();
	for (int i = 0; i < windowCount; ++i)
		randomOrder.push_back(i);
	std::random_shuffle(randomOrder.begin(), randomOrder.end());
	this->currentIndex = 0;
	this->userImageSequenceRecords = director->GetUserImageSequenceRecords();

	this->nextAppearElapsedTime = 0.0f;
	this->startTime = std::chrono::high_resolution_clock::now();
}

BroadcastState::~BroadcastState()
{
}

void BroadcastState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > BROADCAST_STATE_TIME) {
		director->SetInteractionState(std::make_shared<EndingBuddhaState>(director));
		return;
	}
	else if (elapsedTime > nextAppearElapsedTime) {
		if (currentIndex < randomOrder.size()) {
			std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
				userImageSequenceRecords->at(currentIndex % userImageSequenceRecords->size()), USER_VIDEO_TIME, true, true);
			int row = randomOrder[currentIndex] / COL_COUNT;
			int col = randomOrder[currentIndex] % COL_COUNT;
			SetBlendingVideo(row, col, newVideo);

			currentIndex++;
			nextAppearElapsedTime += BROADCAST_TIME_FOR_EACH;
		}
	}
}

std::string BroadcastState::ToString()
{
	return "BroadcastState";
}

void BroadcastState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	// TODO: replace by wave
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		WAVE_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}
