/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "BuddhaState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "BlendingTransitionVideo.h"



#include "IntroInitialState.h"

BuddhaState::BuddhaState(Director *director)
	: InteractionState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	  ROW_CENTER(Setting::GetInstance()->GetCenterRow()),
	  COL_CENTER(Setting::GetInstance()->GetCenterCol()),
	  MAX_DISTANCE_TO_CENTER(Setting::GetInstance()->GetMaxDistanceToCenter()),
	  APEAR_TIME_FOR_EACH(APEAR_TIME / Setting::GetInstance()->GetMaxDistanceToCenter())
{
	this->videoPool = VideoPool::GetInstance();
	this->buddhaVideo = std::make_shared<VideoClip>(videoPool->GetNextBuddhaVideo(), BUDDHA_VIDEO_TIME, false, false);
	this->maxManhattanDistanceToCenter = GetMaxManhattanDistanceToCenter();

	this->currentDistance = 0;

	this->nextAppearElapsedTime = 0.0f;
	this->startTime = std::chrono::high_resolution_clock::now();
}

BuddhaState::~BuddhaState()
{
}

void BuddhaState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > BUDDHA_STATE_TIME) {
		director->SetInteractionState(std::make_shared<IntroInitialState>(director));
		return;
	}
	else if (elapsedTime > nextAppearElapsedTime) {
		for (int row = 0; row < ROW_COUNT; ++row) {
			for (int col = 0; col < COL_COUNT; ++col) {
				if (currentDistance > maxManhattanDistanceToCenter)
					break;
				if (currentDistance == CalculateDistanceToCenter(row, col)) {
					SetBlendingVideo(row, col, buddhaVideo);
				}
			}
		}
		currentDistance++;
		nextAppearElapsedTime += APEAR_TIME_FOR_EACH;
	}
}

std::string BuddhaState::ToString()
{
	return "BuddhaState";
}

int BuddhaState::GetMaxManhattanDistanceToCenter()
{
	int ret = 0;
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			ret = std::max(ret, CalculateDistanceToCenter(row, col));
		}
	}
	
	return ret;
}

int BuddhaState::CalculateDistanceToCenter(int row, int col)
{
	return std::abs(row - ROW_CENTER) + std::abs(col - COL_CENTER);
}

void BuddhaState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}
