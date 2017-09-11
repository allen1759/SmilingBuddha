/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_INITIAL_STATE_H
#define _GAZE_INITIAL_STATE_H

#include "GazeState.h"



//cpp
#include "GazeWanderState.h"

class GazeInitialState : public GazeState
{
public:
	GazeInitialState(Director *director)
		: GazeState(director, std::chrono::high_resolution_clock::now())
	{
	}

	virtual ~GazeInitialState()
	{
	}

	virtual void Update() override
	{
		director->SetInteractionState(std::make_shared<GazeWanderState>(director, GazeState::startTime));
		return;
	}

	virtual std::string ToString() override
	{
		return "GazeInitialState";
	}

private:

};

#endif // !_GAZE_INITIAL_STATE_H
