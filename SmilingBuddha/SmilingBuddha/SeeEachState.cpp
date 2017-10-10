/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SeeEachState.h"
#include "Director.h"

#include <cstdlib>
#include "Setting.h"
#include "SeeEachSmileProcessStrategy.h"
#include "VideoPool.h"
#include "VideoClip.h"
#include "BlendingTransition.h"

SeeEachState::SeeEachState(Director *director)
	: InteractionState(director)
{
	this->videoPool = VideoPool::GetInstance();
	Setting *setting = Setting::GetInstance();
	this->rowCount = setting->GetRow();
	this->colCount = setting->GetCol();
	this->centerRow = setting->GetCenterRow();
	this->centerCol = setting->GetCenterCol();

	this->isPlayingSeeEachAnimation = false;
	this->isSmiling = false;
	this->isPlayingSmilingAnimation = false;

	// TODO: delete
	director->GetSmileVideoProcessor()->SetSmileProcessStrategy(
		std::make_shared<SeeEachSmileProcessStrategy>(shared_from_this(), SEE_EACH_STATE_TIME));

	InitializeVideoGrid();
	this->startTime = std::chrono::high_resolution_clock::now();
	this->animationDuration = std::chrono::milliseconds(static_cast<int>(ANIMATION_TIME * 1000));
}

SeeEachState::~SeeEachState()
{
	// set to null
}

void SeeEachState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

	if (!isPlayingSmilingAnimation) {
		if (delta >= animationDuration) {
			if (isPlayingSeeEachAnimation) {
				// Set 'from' grid to Neutral.
				std::shared_ptr<Video> newVideo = GetVideoClipByDirection(lastFromRow, lastFromCol, ActorVideoSet::NEUTRAL, true, true);
				SetBlendingVideo(lastFromRow, lastFromCol, newVideo);

				// Set 'at' grid to Neutral.
				newVideo = GetVideoClipByDirection(lastAtRow, lastAtCol, ActorVideoSet::NEUTRAL, true, true);
				SetBlendingVideo(lastAtRow, lastAtCol, newVideo);

				isPlayingSeeEachAnimation = false;
			}

			if (rand() % 10 < ANIMATION_PROBABILITY) {
				SetSeeEachAnimation();
			}

			startTime = std::chrono::high_resolution_clock::now();
		}
		else if (isSmiling) {
			// do smiling animation
			for (int i = 0; i < SQUARE_SIZE; ++i) {
				int row = centerRow + DIRECTION[i][1];
				int col = centerCol + DIRECTION[i][0];
				int lookDirection = ActorVideoSet::GetDirectionIndex(row, col, centerRow, centerCol);
				std::shared_ptr<Video> newVideo = GetVideoClipByDirection(row, col, lookDirection, false, true);
				SetBlendingVideo(row, col, newVideo);
			}

			isPlayingSmilingAnimation = true;
			startTime = std::chrono::high_resolution_clock::now();
		}
	}
	
	if (isPlayingSmilingAnimation) {
		if (delta >= animationDuration) {
			// Set all actor to Neutual direction.
			for (int i = 0; i < SQUARE_SIZE; ++i) {
				int row = centerRow + DIRECTION[i][1];
				int col = centerCol + DIRECTION[i][0];
				std::shared_ptr<Video> newVideo = GetVideoClipByDirection(row, col, ActorVideoSet::NEUTRAL, true, true);
				SetBlendingVideo(row, col, newVideo);
			}
		}
	}
}

void SeeEachState::InitializeVideoGrid()
{
	for (int i = 0; i < SQUARE_SIZE; ++i) {
		int row = centerRow + DIRECTION[i][1];
		int col = centerCol + DIRECTION[i][0];
		if (col < 0 || col >= colCount)
			continue;
		if (row < 0 || row >= rowCount)
			continue;

		std::shared_ptr<Video> newVideo = GetVideoClipByDirection(row, col, ActorVideoSet::NEUTRAL, true, true);
		director->GetVideoGrid()->SetChild(newVideo, row, col);
	}
}

void SeeEachState::SetSeeEachAnimation()
{
	int animationIndex = rand() % SQUARE_SIZE;
	int row = centerRow + DIRECTION[animationIndex][1];
	int col = centerCol + DIRECTION[animationIndex][0];

	int anotherRow, anotherCol, relativeDirection;
	int randIndex = rand() % (SQUARE_SIZE - 1);
	for (int i = 0; i < (SQUARE_SIZE - 1); ++i) {
		relativeDirection = (randIndex + i) % (SQUARE_SIZE - 1);
		anotherRow = row + NEAR_BY_DIRECTION[relativeDirection][1];
		anotherCol = col + NEAR_BY_DIRECTION[relativeDirection][0];

		if (std::abs(anotherRow - centerRow) <= 1 &&
			std::abs(anotherCol - centerCol) <= 1)
			break;
	}

	// Find out the looking direction and inverse looking direction.
	int lookDirection = ActorVideoSet::GetDirectionIndex(row, col, anotherRow, anotherCol);
	int anotherLookDirection = ActorVideoSet::GetDirectionIndex(anotherRow, anotherCol, row, col);

	// Set 'frome' grid new video.
	std::shared_ptr<Video> newVideo = GetVideoClipByDirection(row, col, lookDirection, false, true);
	SetBlendingVideo(row, col, newVideo);

	// Set 'at' grid new video.
	std::shared_ptr<Video> anotherNewVideo = GetVideoClipByDirection(anotherRow, anotherCol, anotherLookDirection, false, true);
	SetBlendingVideo(anotherRow, anotherCol, anotherNewVideo);

	isPlayingSeeEachAnimation = true;
	lastFromRow = row;
	lastFromCol = col;
	lastAtRow = anotherRow;
	lastAtCol = anotherCol;
}

std::shared_ptr<Video> SeeEachState::GetVideoClipByDirection(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		ANIMATION_TIME, loop, reverse);

	return newVideo;
}

void SeeEachState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransition>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME
		);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}

void SeeEachState::OnSmile()
{
	// TODO: play audio.
}

void SeeEachState::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
}
