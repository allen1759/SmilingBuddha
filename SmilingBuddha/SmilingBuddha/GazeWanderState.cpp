/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeWanderState.h"

#include "GazeLockState.h"

void GazeWanderState::Update()
{
	GazeState::Update();

	if (GazeState::switchToBroadcastState) {
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
	HeadPost2RowCol(director->GetHeadPose(), gazeRow, gazeCol);
	if ((gazeRow != lastGazeRow || gazeCol != lastGazeCol) && 
		gazeRow >= 0 && gazeRow < ROW_COUNT && gazeCol >= 0 && gazeCol < COL_COUNT) {
		lastSelectTime = currentTime;
		lastGazeRow = gazeRow;
		lastGazeCol = gazeCol;
	}
}
