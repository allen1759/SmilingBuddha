/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeWanderState.h"

#include "Setting.h"
#include "Director.h"
#include "ChangeBackgroundAnimatedVideo.h"
#include "GazeLockState.h"
#include "BroadcastState.h"

GazeWanderState::GazeWanderState(Director * director, std::chrono::high_resolution_clock::time_point startTime)
	: GazeState(director, startTime)
{
	this->lastSelectTime = std::chrono::high_resolution_clock::now();

	// Set ChangeBackgroundAnimatedVideo at center grid.
	director->GetVideoGrid()->SetChild(
		std::make_shared<ChangeBackgroundAnimatedVideo>(
			director->GetVideoGrid()->GetChild(GazeState::lastGazeRow, GazeState::lastGazeCol), GazeState::CHANGE_BACKGROUND_TIME),
		GazeState::lastGazeRow, GazeState::lastGazeCol);
}

GazeWanderState::~GazeWanderState()
{
}

void GazeWanderState::Update()
{
	GazeState::Update();

	// Check whether it's time to change state or not.
	if (GazeState::switchToBroadcastState) {
		// Clear ChangeBackgroundAnimation.
		director->GetVideoGrid()->SetChild(
			director->GetVideoGrid()->GetChild(GazeState::lastGazeRow, GazeState::lastGazeRow)->GetVideo(),
			GazeState::lastGazeRow, GazeState::lastGazeCol);
		director->SetInteractionState(std::make_shared<BroadcastState>(director));
		return;
	}

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastSelectTime).count();
	if (elapsedTime > MAX_STARING_TIME) {
		// Change to GazeLockState.
		director->SetInteractionState(std::make_shared<GazeLockState>(director, GazeState::startTime, GazeState::lastGazeRow, GazeState::lastGazeCol));
		return;
	}

	int gazeRow, gazeCol;
	GazeState::HeadPost2RowCol(director->GetHeadPose(), gazeRow, gazeCol);

	// If gaze grid changed, update ChangeBackgroundAnimation position.
	if ((gazeRow != GazeState::lastGazeRow || gazeCol != GazeState::lastGazeCol)) {
		GazeState::MoveChangeBackgroundAnimationToOtherGrid(gazeRow, gazeCol);
		// Record the select time.
		lastSelectTime = currentTime;
	}
}

std::string GazeWanderState::ToString()
{
	return "GazeWanderState";
}
