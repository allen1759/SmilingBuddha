/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeState.h"

#include "Director.h"
#include "VideoClip.h"
#include "BroadcastState.h"

GazeState::GazeState(Director *director, std::chrono::high_resolution_clock::time_point startTime)
	: InteractionState(director)
{
	this->videoPool = VideoPool::GetInstance();

	this->switchToBroadcastState = false;
	this->userImages = NULL;

	this->elapsedTime = 0.0f;
	this->startTime = startTime;
}

GazeState::~GazeState()
{
}

void GazeState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > GAZE_STATE_TIME)
		switchToBroadcastState = true;
}

std::shared_ptr<Video> GazeState::GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		ACTOR_VIDEO_TIME, loop, reverse);

	return newVideo;
}
