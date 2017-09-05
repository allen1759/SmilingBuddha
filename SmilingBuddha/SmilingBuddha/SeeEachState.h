/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SEE_EACH_STATE_H
#define _SEE_EACH_STATE_H

#include "InteractionState.h"
#include "SmileObserver.h"

#include <chrono>

#include "VideoPool.h"
#include "Video.h"

class Director;

class SeeEachState : public InteractionState, public SmileObserver
{
public:
	SeeEachState(Director *director);

	virtual ~SeeEachState();

	virtual void Update() override;

private:
	void InitializeVideoGrid();

	void SetSeeEachAnimation();

	std::shared_ptr<Video> GetVideoClipByDirection(int row, int col, int direction, bool loop, bool reverse);

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);

	virtual void OnSmile() override;

	virtual void OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images) override;


	// Get from Setting class.
	VideoPool *videoPool;
	int rowCount;
	int colCount;
	int centerRow;
	int centerCol;

	// Initialize only 9 grids around center.
	const int SQUARE_SIZE = 9;
	int DIRECTION[9][2] = { {-1, -1}, {-1, 0}, {-1, 1},
							{0, -1}, {0, 0}, {0, 1},
							{1, -1}, {1, 0}, {1, 1} };
	int NEAR_BY_DIRECTION[8][2] = { {-1, -1}, {-1, 0}, {-1, 1},
									{0, -1}, {0, 1},
									{1, -1}, {1, 0}, {1, 1} };
	const float SEE_EACH_STATE_TIME = 20.0f;
	const float ANIMATION_TIME = 6.0f;
	const float BLENDING_TIME = 0.5f;
	// With 80% probability to create SeeEach animation.
	const int ANIMATION_PROBABILITY = 8;

	bool isPlayingSeeEachAnimation;
	int lastFromRow, lastFromCol;
	int lastAtRow, lastAtCol;

	bool isSmiling;
	bool isPlayingSmilingAnimation;

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::milliseconds animationDuration;
};


#endif // !_SEE_EACH_STATE_H
