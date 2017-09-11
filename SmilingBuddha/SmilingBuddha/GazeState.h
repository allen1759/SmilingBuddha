/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_STATE_H
#define _GAZE_STATE_H

#include "InteractionState.h"

#include <chrono>

#include "VideoPool.h"
#include "Video.h"

class GazeState : public InteractionState
{
public:
	GazeState(Director *director, std::chrono::high_resolution_clock::time_point startTime);

	virtual ~GazeState();

	virtual void Update() override;

protected:
	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse);


	const float GAZE_STATE_TIME = 2000.0f;
	const float ACTOR_VIDEO_TIME = 3.0f;

	VideoPool *videoPool;

	bool switchToBroadcastState;
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> userImages;

	float elapsedTime;
	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_GAZE_STATE_H
