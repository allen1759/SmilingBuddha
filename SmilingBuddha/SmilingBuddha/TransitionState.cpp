/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "TransitionState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"

// replace by wave animation
#include "BlendingTransitionVideo.h"
#include "IntroInitialState.h"
#include "BroadcastState.h"

TransitionState::TransitionState(Director *director)
	: InteractionState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	  MAX_DISTANCE_TO_CENTER(Setting::GetInstance()->GetMaxDistanceToCenter())
{
	this->videoPool = VideoPool::GetInstance();

	this->currentDistance = MAX_DISTANCE_TO_CENTER;

	this->currentDelayTime = INITIAL_DELAY_TIME;
	this->nextAppearElapsedTime = currentDelayTime;
	this->startTime = std::chrono::high_resolution_clock::now();
}

TransitionState::~TransitionState()
{
}

void TransitionState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > nextAppearElapsedTime) {
		if (currentDistance == Setting::GetInstance()->GetMaxDistanceToCenterInGrid()) {
			director->SetInteractionState(std::make_shared<BroadcastState>(director));
			return;
		}

		for (int row = 0; row < ROW_COUNT; ++row) {
			for (int col = 0; col < COL_COUNT; ++col) {
				if (currentDistance == Setting::GetInstance()->CalculateDistanceToCenter(row, col)) {
					AppearVideo(row, col);
				}
			}
		}

		currentDistance--;
		nextAppearElapsedTime += currentDelayTime;
		currentDelayTime /= 2.0f;
	}
}

std::string TransitionState::ToString()
{
	return "TransitionState";
}

void TransitionState::AppearVideo(int row, int col)
{
	if (Setting::GetInstance()->IsInIntroStateGrid(row, col))
		return;

	std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
	director->GetVideoGrid()->SetChild(newVideo, row, col);
}

std::shared_ptr<Video> TransitionState::GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		ACTOR_VIDEO_TIME, loop, reverse);

	return newVideo;
}

