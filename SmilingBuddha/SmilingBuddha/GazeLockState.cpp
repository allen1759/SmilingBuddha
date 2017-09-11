/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeLockState.h"

#include "GazeWanderState.h"

void GazeLockState::Update()
{
	GazeState::Update();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lockStartTime).count();

	if (!isStartSeeGaze && elapsedTime > startSeeGazeElapsedTime) {
		SetAllSeeGazeVideo();
		isStartSeeGaze = true;
	}
	else if (!isStartSeeBack && elapsedTime > startSeeBackElapsedTime) {
		std::shared_ptr<Video> newVideo = GetActorDirectionVideo(gazeRow, gazeCol, ActorVideoSet::NEUTRAL, true, true);
		SetWaveVideo(gazeRow, gazeCol, newVideo);
		isStartSeeBack = true;
	}
	else if (elapsedTime > endSeeBackElapsedTime) {
		SetNeutralVideo();
		director->SetInteractionState(std::make_shared<GazeWanderState>(director, GazeState::startTime));
	}
}
