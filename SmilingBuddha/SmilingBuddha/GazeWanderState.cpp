/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeWanderState.h"

#include "Setting.h"
#include "ChangeBackgroundAnimatedVideo.h"
#include "GazeLockState.h"
#include "BroadcastState.h"

GazeWanderState::GazeWanderState(Director * director, std::chrono::high_resolution_clock::time_point startTime)
	: GazeState(director, startTime)

{
	this->lastGazeRow = ROW_CENTER;
	this->lastGazeCol = COL_CENTER;
	this->lastSelectTime = std::chrono::high_resolution_clock::now();

	director->GetVideoGrid()->SetChild(
		std::make_shared<ChangeBackgroundAnimatedVideo>(
			director->GetVideoGrid()->GetChild(lastGazeRow, lastGazeCol), MAX_STARING_TIME),
		lastGazeRow, lastGazeCol);
}

GazeWanderState::~GazeWanderState()
{
}

void GazeWanderState::Update()
{
	GazeState::Update();

	if (GazeState::switchToBroadcastState) {
		director->GetVideoGrid()->SetChild(
			director->GetVideoGrid()->GetChild(lastGazeRow, lastGazeRow)->GetVideo(),
			lastGazeRow, lastGazeCol);
		director->SetInteractionState(std::make_shared<BroadcastState>(director));
		return;
	}

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastSelectTime).count();
	if (elapsedTime > MAX_STARING_TIME) {
		director->SetInteractionState(std::make_shared<GazeLockState>(director, GazeState::startTime, lastGazeRow, lastGazeCol));
		return;
	}

	int gazeRow, gazeCol;
	GazeState::HeadPost2RowCol(director->GetHeadPose(), gazeRow, gazeCol);

	if ((gazeRow != lastGazeRow || gazeCol != lastGazeCol) && 
		gazeRow >= 0 && gazeRow < ROW_COUNT && gazeCol >= 0 && gazeCol < COL_COUNT) {

		director->GetVideoGrid()->SetChild(
			director->GetVideoGrid()->GetChild(lastGazeRow, lastGazeCol)->GetVideo(),
			lastGazeRow, lastGazeCol);
		director->GetVideoGrid()->SetChild(
			std::make_shared<ChangeBackgroundAnimatedVideo>(
				director->GetVideoGrid()->GetChild(gazeRow, gazeCol), MAX_STARING_TIME),
			gazeRow, gazeCol);

		lastSelectTime = currentTime;
		lastGazeRow = gazeRow;
		lastGazeCol = gazeCol;
	}
}

std::string GazeWanderState::ToString()
{
	return "GazeWanderState";
}
