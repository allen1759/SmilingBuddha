/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroSeeEachState.h"

#include "Setting.h"
#include "Director.h"
#include "IntroNeutralState.h"
#include "SmileState.h"
#include "BlendingTransitionVideo.h"

IntroSeeEachState::IntroSeeEachState(Director *director)
	: IntroState(director),
	  CENTER_ROW(Setting::GetInstance()->GetCenterRow()),
	  CENTER_COL(Setting::GetInstance()->GetCenterCol())
{
	this->endingElapsedTime = IntroState::ACTOR_VIDEO_TIME * 2;

	SetSeeEachVideo();
}

IntroSeeEachState::~IntroSeeEachState()
{
}

void IntroSeeEachState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - IntroState::startTime).count();

	if (elapsedTime > endingElapsedTime) {
		if (IntroState::switchToSmileState) {
			director->SetInteractionState(std::make_shared<SmileState>(director, userImages));
			return;
		}
		else {
			// Set 2 VideoGrid to neutral video.
			std::shared_ptr<Video> newVideo = GetActorDirectionVideo(lastFromRow, lastFromCol, ActorVideoSet::NEUTRAL, true, true);
			SetBlendingVideo(lastFromRow, lastFromCol, newVideo);
			newVideo = GetActorDirectionVideo(lastAtRow, lastAtCol, ActorVideoSet::NEUTRAL, true, true);
			SetBlendingVideo(lastAtRow, lastAtCol, newVideo);

			// Return to NeutralState.
			director->SetInteractionState(std::make_shared<IntroNeutralState>(director));
			return;
		}
	}
}

std::string IntroSeeEachState::ToString()
{
	return "IntroSeeEachState";
}

void IntroSeeEachState::SetSeeEachVideo()
{
	int row, col, anotherRow, anotherCol;
	Setting::GetInstance()->GetPairRowColInIntroStateGrid(row, col, anotherRow, anotherCol);


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

void IntroSeeEachState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}
