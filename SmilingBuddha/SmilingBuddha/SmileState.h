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

	void SetSeeCenterAnimation();

	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse);

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	// Get from Setting class.
	const int ROW_COUNT;
	const int COL_COUNT;
	const int CENTER_ROW;
	const int CENTER_COL;

	// Initialize only 9 grids around center.
	const int SQUARE_SIZE = 9;
	int DIRECTION[9][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
	{ 0, -1 },{ 0, 0 },{ 0, 1 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 } };
	int NEAR_BY_DIRECTION[8][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
	{ 0, -1 },{ 0, 1 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 } };

	const float WAVE_TIME = 3.0f;
	const float USER_TIME = 3.0f;
	const float ALL_SEE_TIME = 6.0f;
	const float BLENDING_TIME = 0.5f;

	VideoPool *videoPool;

	bool isStartSeeCenter;
	bool isStartSeeBack;

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::milliseconds startSeeCenterElapsedTime;
	std::chrono::milliseconds startSeeBackElapsedTime;
	std::chrono::milliseconds endSeeBackElapsedTime;
};

#endif // !_SMILE_STATE_H
