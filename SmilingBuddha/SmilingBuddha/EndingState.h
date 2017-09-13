/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ENDING_STATE_H
#define _ENDING_STATE_H

#include "InteractionState.h"

#include <chrono>

#include "VideoPool.h"

class EndingState : public InteractionState
{
public:
	EndingState(Director *director);

	virtual ~EndingState();

protected:
	void AppearAnimation(int dist);

	virtual void SetTransition(int row, int col) = 0;

private:
	int GetMaxManhattanDistanceToCenter();


protected:
	VideoPool *videoPool;

	const int ROW_COUNT;
	const int COL_COUNT;
	const int ROW_CENTER;
	const int COL_CENTER;
	const int MAX_DISTANCE_TO_CENTER;

	const float APEAR_TIME = 3.0f;
	const float APEAR_TIME_FOR_EACH;

	int maxManhattanDistanceToCenter;

	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_ENDING_STATE_H
