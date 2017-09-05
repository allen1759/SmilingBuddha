/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroSeeEachState.h"

#include "Director.h"

IntroSeeEachState::IntroSeeEachState(Director *director)
	:IntroState(director)
{
	this->animationDuration = std::chrono::milliseconds(static_cast<int>(VIDEO_TIME * 1000));

	SetSmileAnimation();
}

IntroSeeEachState::~IntroSeeEachState()
{
}

void IntroSeeEachState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

	if (delta > animationDuration) {
		std::shared_ptr<Video> newVideo = GetActorDirectionVideo(lastFromRow, lastFromCol, ActorVideoSet::NEUTRAL, true, true);
		SetBlendingVideo(lastFromRow, lastFromCol, newVideo);
		newVideo = GetActorDirectionVideo(lastAtRow, lastAtCol, ActorVideoSet::NEUTRAL, true, true);
		SetBlendingVideo(lastAtRow, lastAtCol, newVideo);

		director->SetInteractionState(std::make_shared<IntroNeutralState>(director));
	}
}

std::string IntroSeeEachState::ToString()
{
	return "IntroSeeEachState";
}

void IntroSeeEachState::SetSmileAnimation()
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
	std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, lookDirection, false, true);
	SetBlendingVideo(row, col, newVideo);

	// Set 'at' grid new video.
	std::shared_ptr<Video> anotherNewVideo = GetActorDirectionVideo(anotherRow, anotherCol, anotherLookDirection, false, true);
	SetBlendingVideo(anotherRow, anotherCol, anotherNewVideo);

	lastFromRow = row;
	lastFromCol = col;
	lastAtRow = anotherRow;
	lastAtCol = anotherCol;
}

