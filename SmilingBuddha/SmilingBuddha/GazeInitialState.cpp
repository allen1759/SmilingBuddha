/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeInitialState.h"

#include "Setting.h"
#include "Director.h"
#include "ChangeBackgroundAnimatedVideo.h"
#include "GazeWanderState.h"

GazeInitialState::GazeInitialState(Director * director)
	: GazeState(director, std::chrono::high_resolution_clock::now()),
	  MAX_DISTANCE_TO_CENTER(Setting::GetInstance()->GetMaxDistanceToCenter())
{
	this->currentDistance = MAX_DISTANCE_TO_CENTER;
	this->currentDelayTime = INITIAL_DELAY_TIME;
	this->nextAppearElapsedTime = currentDelayTime;

	// Set ChangeBackgroundAnimatedVideo at center grid.
	director->GetVideoGrid()->SetChild(
		std::make_shared<ChangeBackgroundAnimatedVideo>(
			director->GetVideoGrid()->GetChild(GazeState::lastGazeRow, GazeState::lastGazeCol), GazeState::CHANGE_BACKGROUND_TIME),
		GazeState::lastGazeRow, GazeState::lastGazeCol);
}

GazeInitialState::~GazeInitialState()
{
}

void GazeInitialState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	// Controll incremental appearance of grids.
	if (elapsedTime > nextAppearElapsedTime) {
		// If appearance finished, change to GazeWanderState.
		if (currentDistance == Setting::GetInstance()->GetMaxDistanceToCenterInGrid()) {
			// Clear ChangeBackgroundAnimation.
			director->GetVideoGrid()->SetChild(
				director->GetVideoGrid()->GetChild(GazeState::lastGazeRow, GazeState::lastGazeCol)->GetVideo(),
				GazeState::lastGazeRow, GazeState::lastGazeCol);
			director->SetInteractionState(std::make_shared<GazeWanderState>(director, std::chrono::high_resolution_clock::now()));
			return;
		}
		// Grid appearance.
		for (int row = 0; row < ROW_COUNT; ++row) {
			for (int col = 0; col < COL_COUNT; ++col) {
				if (currentDistance == Setting::GetInstance()->CalculateDistanceToCenter(row, col))
					AppearVideo(row, col);
			}
		}

		currentDistance--;
		nextAppearElapsedTime += currentDelayTime;
		currentDelayTime /= 2.0f;
	}

	// Controll ChangeBackgroundAnimatedVideo of gaze grid.
	int gazeRow, gazeCol;
	GazeState::HeadPost2RowCol(director->GetHeadPose(), gazeRow, gazeCol);

	// If gaze grid changed, update ChangeBackgroundAnimation position.
	if ((gazeRow != GazeState::lastGazeRow || gazeCol != GazeState::lastGazeCol))
		GazeState::MoveChangeBackgroundAnimationToOtherGrid(gazeRow, gazeCol);
}

std::string GazeInitialState::ToString()
{
	return "GazeInitialState";
}

void GazeInitialState::AppearVideo(int row, int col)
{
	// If the grid has already been set, ignore it.
	if (director->GetVideoGrid()->GetChild(row, col) != NULL)
		return;

	std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
	director->GetVideoGrid()->SetChild(newVideo, row, col);
}
