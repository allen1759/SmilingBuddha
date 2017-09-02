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
	VideoPool *videoPool = VideoPool::GetInstance();
	int rowCount = Setting::GetInstance()->GetRow();
	int colCount = Setting::GetInstance()->GetCol();
	int centerRow = Setting::GetInstance()->GetCenterRow();
	int centerCol = Setting::GetInstance()->GetCenterRow();

	isPlayingAnimation = false;

	director->GetSmileVideoProcessor()->SetSmileProcessStrategy(
		std::make_shared<SeeEachSmileProcessStrategy>(director, SEE_EACH_STATE_TIME));

	InitializeVideoGrid();
	this->startTime = std::chrono::high_resolution_clock::now();
	this->animationDuration = std::chrono::milliseconds(static_cast<int>(ANIMATION_TIME * 1000));
}

SeeEachState::~SeeEachState()
{
}

void SeeEachState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

	if (delta >= animationDuration) {
		if (isPlayingAnimation) {
			// Set 'from' grid to Neutral.
			std::shared_ptr<Video> newVideo = CreateNeutralVideo(lastFromRow, lastFromCol);
			director->GetVideoGrid()->SetChild(newVideo, lastFromRow, lastFromCol);
			// Set 'at' grid to Neutral.
			newVideo = CreateNeutralVideo(lastAtRow, lastAtCol);
			director->GetVideoGrid()->SetChild(newVideo, lastAtRow, lastAtCol);

			isPlayingAnimation = false;
		}
		if (rand() % 10 < ANIMATION_PROBABILITY) {
			CreateSeeEachAnimation();
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

		std::shared_ptr<Video> newVideo = CreateNeutralVideo(row, col);
		director->GetVideoGrid()->SetChild(newVideo, row, col);
	}
}

void SeeEachState::SetNeutralBlending(int row, int col)
{
	std::shared_ptr<Video> newVideo = CreateNeutralVideo(row, col);
	std::shared_ptr<Video> transition = std::make_shared<BlendingTransition>(
		director->GetVideoGrid()->GetChild(row, col),
		newVideo,
		BLENDING_TIME);
	director->GetVideoGrid()->SetChild(transition, row, col);
}

std::shared_ptr<Video> SeeEachState::CreateNeutralVideo(int row, int col)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(ActorVideoSet::NEUTRAL),
		ANIMATION_TIME, true, true);
	
	return newVideo;
}

void SeeEachState::CreateSeeEachAnimation()
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
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(lookDirection),
		ANIMATION_TIME, false, true);
	director->GetVideoGrid()->SetChild(newVideo, row, col);

	// Set 'at' grid new video.
	actorVideoSet = videoPool->GetActorVideoSet(anotherRow, anotherCol);
	std::shared_ptr<Video> anotherNewVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(anotherLookDirection),
		ANIMATION_TIME, false, true);
	director->GetVideoGrid()->SetChild(anotherNewVideo, anotherRow, anotherCol);

	isPlayingAnimation = true;
	lastFromRow = row;
	lastFromCol = col;
	lastAtRow = anotherRow;
	lastAtCol = anotherCol;
}
