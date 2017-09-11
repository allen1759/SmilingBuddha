/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_WANDER_STATE_H
#define _GAZE_WANDER_STATE_H

#include "GazeState.h"

#include "Ray.h"

//cpp
#include "Setting.h"
#include "BroadcastState.h"

class GazeWanderState : public GazeState
{
public:
	GazeWanderState(Director *director, std::chrono::high_resolution_clock::time_point startTime);

	virtual ~GazeWanderState()
	{
	}

	virtual void Update() override;

	virtual std::string ToString() override
	{
		return "GazeWanderState";
	}

private:
	void HeadPost2RowCol(Ray headPose, int &row, int &col)
	{
		glm::vec3 origin = headPose.GetOrigin();
		glm::vec3 direction = headPose.GetDirection();

		// Check boundary case.
		if (direction.z == 0)
			direction.z = 1e-9;

		float multiply = -origin.z / direction.z;
		glm::vec3 intersection = origin + direction * multiply;

		float rowMeter = -intersection.y + PROJECTION_HEIGHT / 2;
		float colMeter = intersection.x + PROJECTION_WIDTH / 2;
		std::cout << "Meter row: " << rowMeter << " col: " << colMeter << std::endl;

		row = rowMeter / PROJECTION_HEIGHT * ROW_COUNT;
		col = colMeter / PROJECTION_WIDTH * COL_COUNT;

		row = std::max(0, std::min(ROW_COUNT - 1, row));
		col = std::max(0, std::min(COL_COUNT - 1, col));
	}

	const int ROW_COUNT;
	const int COL_COUNT;
	const int ROW_CENTER;
	const int COL_CENTER;

	const float PROJECTION_WIDTH;
	const float PROJECTION_HEIGHT;

	const float MAX_STARING_TIME = 3.0f;

	int lastGazeRow;
	int lastGazeCol;
	std::chrono::high_resolution_clock::time_point lastSelectTime;
};

#endif // !_GAZE_WANDER_STATE_H
