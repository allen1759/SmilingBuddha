/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_WANDER_STATE_H
#define _GAZE_WANDER_STATE_H

#include "GazeState.h"

class GazeWanderState : public GazeState
{
public:
	GazeWanderState(Director *director, std::chrono::high_resolution_clock::time_point startTime);

	virtual ~GazeWanderState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:






	const float MAX_STARING_TIME = 3.0f;

	int lastGazeRow;
	int lastGazeCol;
	std::chrono::high_resolution_clock::time_point lastSelectTime;
};

#endif // !_GAZE_WANDER_STATE_H
