/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _BROADCAST_STATE_H
#define _BROADCAST_STATE_H

#include "InteractionState.h"

#include <vector>
#include <chrono>

#include "Video.h"

class BroadcastState : public InteractionState
{
public:
	BroadcastState(Director *director);

	virtual ~BroadcastState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	const int COL_COUNT;
	const float BROADCAST_STATE_TIME = 28.0f;
	const float ALL_APPEAR_TIME = 8.0f;
	const float APPEAR_TIME_FOR_EACH;
	const float USER_VIDEO_TIME = 3.0f;
	const float WAVE_TIME = 3.0f;

	std::vector<int> randomOrder;
	int currentIndex;
	std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>> userImageSequenceRecords;

	float nextAppearElapsedTime;
	std::chrono::high_resolution_clock::time_point startTime;

};


#endif // !_BROADCAST_STATE_H
