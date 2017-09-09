/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_STATE_H
#define _SMILE_STATE_H

#include "InteractionState.h"

#include <chrono>

#include "VideoPool.h"
#include "Video.h"

class SmileState : public InteractionState
{
public:
	SmileState(Director *director, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images);

	~SmileState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	void SetWaveAnimationByImageSequenced(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images);

	void SetWaveAnimationByOriginVideo();

	void SetSeeCenterVideo();

	void SetNeutralVideo();

	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int directione, bool loop, bool reverse);

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	// Get from Setting class.
	const int ROW_COUNT;
	const int COL_COUNT;
	const int CENTER_ROW;
	const int CENTER_COL;

	const float USER_VIDEO_TIME = 3.0f;
	const float ACTOR_VIDEO_TIME = 3.0f;
	const float WAVE_TIME = 3.0f;
	const float BLENDING_TIME = 0.5f;

	VideoPool *videoPool;

	bool isStartSeeCenter;
	bool isStartSeeBack;

	float startSeeCenterElapsedTime;
	float startSeeBackElapsedTime;
	float endSeeBackElapsedTime;
	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_SMILE_STATE_H
