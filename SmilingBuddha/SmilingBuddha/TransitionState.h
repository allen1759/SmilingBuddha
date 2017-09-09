/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _TRANSITION_STATE_H
#define _TRANSITION_STATE_H

#include "InteractionState.h"

#include <chrono>

#include "VideoPool.h"
#include "Video.h"

class TransitionState : public InteractionState
{
public:
	TransitionState(Director *director);

	virtual ~TransitionState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	void AppearVideo(int row, int col);

	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse);
	

	const int ROW_COUNT;
	const int COL_COUNT;
	const int MAX_DISTANCE_TO_CENTER;

	const float INITIAL_DELAY_TIME = 2.0f;
	const float ACTOR_VIDEO_TIME = 3.0f;

	VideoPool *videoPool;

	int currentDistance;

	float currentDelayTime;
	float nextAppearElapsedTime;
	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_TRANSITION_STATE_H
