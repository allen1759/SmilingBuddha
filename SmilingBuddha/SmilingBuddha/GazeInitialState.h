/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_INITIAL_STATE_H
#define _GAZE_INITIAL_STATE_H

#include "GazeState.h"



//cpp
#include "ChangeBackgroundAnimatedVideo.h"
#include "GazeWanderState.h"

class GazeInitialState : public GazeState
{
public:
	GazeInitialState(Director *director)
		: GazeState(director, std::chrono::high_resolution_clock::now()),
		  MAX_DISTANCE_TO_CENTER(Setting::GetInstance()->GetMaxDistanceToCenter())
	{
		this->currentDistance = MAX_DISTANCE_TO_CENTER;

		this->currentDelayTime = INITIAL_DELAY_TIME;
		this->nextAppearElapsedTime = currentDelayTime;

		this->lastGazeRow = GazeState::ROW_CENTER;
		this->lastGazeCol = GazeState::COL_CENTER;
		director->GetVideoGrid()->SetChild(
			std::make_shared<ChangeBackgroundAnimatedVideo>(
				director->GetVideoGrid()->GetChild(lastGazeRow, lastGazeCol), CHANGE_BACKGROUND_TIME),
			lastGazeRow, lastGazeCol);
	}

	virtual ~GazeInitialState()
	{
	}

	virtual void Update() override
	{
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

		if (elapsedTime > nextAppearElapsedTime) {
			if (currentDistance == Setting::GetInstance()->GetMaxDistanceToCenterInGrid()) {
				director->SetInteractionState(std::make_shared<GazeWanderState>(director, std::chrono::high_resolution_clock::now()));
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


		int gazeRow, gazeCol;
		GazeState::HeadPost2RowCol(director->GetHeadPose(), gazeRow, gazeCol);

		if ((gazeRow != lastGazeRow || gazeCol != lastGazeCol) &&
			gazeRow >= 0 && gazeRow < ROW_COUNT && gazeCol >= 0 && gazeCol < COL_COUNT) {

			if (director->GetVideoGrid()->GetChild(gazeRow, gazeCol) == NULL)
				return;

			director->GetVideoGrid()->SetChild(
				director->GetVideoGrid()->GetChild(lastGazeRow, lastGazeCol)->GetVideo(),
				lastGazeRow, lastGazeCol);
			director->GetVideoGrid()->SetChild(
				std::make_shared<ChangeBackgroundAnimatedVideo>(
					director->GetVideoGrid()->GetChild(gazeRow, gazeCol), CHANGE_BACKGROUND_TIME),
				gazeRow, gazeCol);

			lastGazeRow = gazeRow;
			lastGazeCol = gazeCol;
		}
	}

	virtual std::string ToString() override
	{
		return "GazeInitialState";
	}

private:
	void AppearVideo(int row, int col)
	{
		if (Setting::GetInstance()->IsInIntroStateGrid(row, col))
			return;

		std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
		director->GetVideoGrid()->SetChild(newVideo, row, col);
	}



	const int MAX_DISTANCE_TO_CENTER;

	const float INITIAL_DELAY_TIME = 2.0f;
	const float ACTOR_VIDEO_TIME = 3.0f;

	int currentDistance;

	float currentDelayTime;
	float nextAppearElapsedTime;



	float CHANGE_BACKGROUND_TIME = 3.0f;
	int lastGazeRow;
	int lastGazeCol;
};

#endif // !_GAZE_INITIAL_STATE_H
