/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "EndingState.h"

#include "Setting.h"

EndingState::EndingState(Director *director)
	: InteractionState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	  ROW_CENTER(Setting::GetInstance()->GetCenterRow()),
	  COL_CENTER(Setting::GetInstance()->GetCenterCol()),
	  MAX_DISTANCE_TO_CENTER(Setting::GetInstance()->GetMaxDistanceToCenter()),
	  APPEAR_TIME_FOR_EACH(APEAR_TIME / Setting::GetInstance()->GetMaxDistanceToCenter())
{
	this->videoPool = VideoPool::GetInstance();

	this->maxManhattanDistanceToCenter = GetMaxManhattanDistanceToCenter();

	this->startTime = std::chrono::high_resolution_clock::now();
}

EndingState::~EndingState()
{
}

void EndingState::AppearAnimation(int dist)
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			if (dist > maxManhattanDistanceToCenter)
				break;
			if (dist == Setting::GetInstance()->CalculateManhattenDistanceToCenter(row, col))
				SetTransition(row, col);
		}
	}
}

int EndingState::GetMaxManhattanDistanceToCenter()
{
	int ret = 0;
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			ret = std::max(ret, Setting::GetInstance()->CalculateManhattenDistanceToCenter(row, col));
		}
	}

	return ret;
}
