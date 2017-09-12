/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_INITIAL_STATE_H
#define _GAZE_INITIAL_STATE_H

#include "GazeState.h"

class GazeInitialState : public GazeState
{
public:
	GazeInitialState(Director *director);

	virtual ~GazeInitialState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	/**
	* Create video from ActorVideoSet, and set the video in grid.
	* If the grid has already been set, ignore it.
	*
	* @param row  the row of grid.
	* @param col  the column of grid.
	*
	*/
	void AppearVideo(int row, int col);


	const int MAX_DISTANCE_TO_CENTER;

	const float INITIAL_DELAY_TIME = 2.0f;

	int currentDistance;
	// Delay time will be divide by 2 every time.
	float currentDelayTime;
	float nextAppearElapsedTime;
};

#endif // !_GAZE_INITIAL_STATE_H
