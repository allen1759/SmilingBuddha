/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTRO_STATE_H
#define _INTRO_STATE_H

#include "InteractionState.h"

#include <chrono>

#include "VideoPool.h"
#include "Video.h"

class IntroState : public InteractionState
{
public:
	IntroState(Director *director);

	virtual ~IntroState();

	virtual void OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images) override;

protected:

	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse);

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	const float INTRO_STATE_TIME = 20.0f;
	const float VIDEO_TIME = 3.0f;
	const float BLENDING_TIME = 0.5f;

	VideoPool *videoPool;

	bool switchToSmileState;
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> userImages;

	std::chrono::high_resolution_clock::time_point startTime;
};


#endif // !_INTRO_STATE_H
