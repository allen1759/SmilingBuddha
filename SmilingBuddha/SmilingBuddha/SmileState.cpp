/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SmileState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "BlendingTransitionVideo.h"


#include "TransitionState.h"

SmileState::SmileState(Director *director, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
	: InteractionState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	  CENTER_ROW(Setting::GetInstance()->GetCenterRow()),
	  CENTER_COL(Setting::GetInstance()->GetCenterCol())

{
	videoPool = VideoPool::GetInstance();

	this->isStartSeeCenter = false;
	this->isStartSeeBack = false;

	this->startSeeCenterElapsedTime = std::chrono::milliseconds(static_cast<int>(USER_VIDEO_TIME * 1000));
	this->startSeeBackElapsedTime = std::chrono::milliseconds(static_cast<int>((USER_VIDEO_TIME + ALL_SEE_TIME / 2) * 1000));
	this->endSeeBackElapsedTime = std::chrono::milliseconds(static_cast<int>((USER_VIDEO_TIME + ALL_SEE_TIME) * 1000));
	this->startTime = std::chrono::high_resolution_clock::now();

	director->SetRegularSmileProcessorStrategy();

	SetWaveAnimationByImageSequenced(images);
}

SmileState::~SmileState()
{
}

void SmileState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

	if (!isStartSeeCenter && delta > startSeeCenterElapsedTime) {
		SetSeeCenterVideo();
		isStartSeeCenter = true;
	}
	else if (!isStartSeeBack && delta > startSeeBackElapsedTime) {
		SetWaveAnimationByOriginVideo();
		isStartSeeBack = true;
	}
	else if (delta > endSeeBackElapsedTime) {
		SetNeutralVideo();
		director->SetInteractionState(std::make_shared<TransitionState>(director));
		return;
	}
}

std::string SmileState::ToString()
{
	return "SmileState";
}

void SmileState::SetWaveAnimationByImageSequenced(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(images, USER_VIDEO_TIME, true, true);
	std::shared_ptr<Video> waveVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(CENTER_ROW, CENTER_COL),
		newVideo, WAVE_TIME);

	director->GetVideoGrid()->SetChild(waveVideo, CENTER_ROW, CENTER_COL);
}

void SmileState::SetWaveAnimationByOriginVideo()
{
	std::shared_ptr<Video> newVideo = GetActorDirectionVideo(CENTER_ROW, CENTER_COL, ActorVideoSet::NEUTRAL, true, true);
	std::shared_ptr<Video> waveVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(CENTER_ROW, CENTER_COL),
		newVideo, WAVE_TIME);

	director->GetVideoGrid()->SetChild(waveVideo, CENTER_ROW, CENTER_COL);
}

void SmileState::SetSeeCenterVideo()
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			// Ignore outside grid.
			if (Setting::GetInstance()->CalculateDistanceToCenter(row, col) > Setting::GetInstance()->GetMaxDistanceToCenter())
				continue;
			// Ignore center grid.
			if (row == CENTER_ROW && col == CENTER_COL)
				continue;

			int lookDirection = ActorVideoSet::GetDirectionIndex(row, col, CENTER_ROW, CENTER_COL);
			std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, lookDirection, false, true);
			SetBlendingVideo(row, col, newVideo);
		}
	}
}

void SmileState::SetNeutralVideo()
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			if (!Setting::GetInstance()->IsInIntroStateGrid(row, col))
				continue;

			std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
			SetBlendingVideo(row, col, newVideo);
		}
	}
}

std::shared_ptr<Video> SmileState::GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		ALL_SEE_TIME, loop, reverse);

	return newVideo;
}

void SmileState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}
