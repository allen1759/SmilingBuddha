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
#include "WaveTransitionalVideo.h"


//#include "TransitionState.h"
#include "GazeInitialState.h"

SmileState::SmileState(Director *director, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
	: InteractionState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	  ROW_CENTER(Setting::GetInstance()->GetCenterRow()),
	  COL_CENTER(Setting::GetInstance()->GetCenterCol())

{
	videoPool = VideoPool::GetInstance();

	this->isStartSeeCenter = false;
	this->isStartSeeBack = false;

	this->startSeeCenterElapsedTime = USER_VIDEO_TIME;
	this->startSeeBackElapsedTime = USER_VIDEO_TIME + ACTOR_VIDEO_TIME;
	this->endSeeBackElapsedTime = USER_VIDEO_TIME + ACTOR_VIDEO_TIME * 2;
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
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (!isStartSeeCenter && elapsedTime > startSeeCenterElapsedTime) {
		SetSeeCenterVideo();
		isStartSeeCenter = true;
	}
	else if (!isStartSeeBack && elapsedTime > startSeeBackElapsedTime) {
		SetWaveAnimationByOriginVideo();
		isStartSeeBack = true;
	}
	else if (elapsedTime > endSeeBackElapsedTime) {
		SetNeutralVideo();
		//director->SetInteractionState(std::make_shared<TransitionState>(director));
		director->SetInteractionState(std::make_shared<GazeInitialState>(director));
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
	std::shared_ptr<Video> waveVideo = std::make_shared<WaveTransitionalVideo>(
		director->GetVideoGrid()->GetChild(ROW_CENTER, COL_CENTER),
		newVideo, WAVE_TIME);

	director->GetVideoGrid()->SetChild(waveVideo, ROW_CENTER, COL_CENTER);
}

void SmileState::SetWaveAnimationByOriginVideo()
{
	std::shared_ptr<Video> newVideo = GetActorDirectionVideo(ROW_CENTER, COL_CENTER, ActorVideoSet::NEUTRAL, true, true);
	std::shared_ptr<Video> waveVideo = std::make_shared<WaveTransitionalVideo>(
		director->GetVideoGrid()->GetChild(ROW_CENTER, COL_CENTER),
		newVideo, WAVE_TIME);

	director->GetVideoGrid()->SetChild(waveVideo, ROW_CENTER, COL_CENTER);
}

void SmileState::SetSeeCenterVideo()
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			// Ignore outside grid.
			if (Setting::GetInstance()->CalculateDistanceToCenter(row, col) > Setting::GetInstance()->GetMaxDistanceToCenterInGrid())
				continue;
			// Ignore center grid.
			if (row == ROW_CENTER && col == COL_CENTER)
				continue;

			int lookDirection = ActorVideoSet::GetDirectionIndex(row, col, ROW_CENTER, COL_CENTER);
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
		ACTOR_VIDEO_TIME, loop, reverse);

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
