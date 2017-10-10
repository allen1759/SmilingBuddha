/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeLockState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "WaveTransitionalVideo.h"
#include "GazeWanderState.h"

GazeLockState::GazeLockState(Director * director, std::chrono::high_resolution_clock::time_point startTime, int gazeRow, int gazeCol)
	: GazeState(director, startTime)
{
	this->isStartSeeGaze = false;
	this->isStartSeeBack = false;
	this->startSeeGazeElapsedTime = USER_VIDEO_TIME;
	this->startSeeBackElapsedTime = USER_VIDEO_TIME + GazeState::GazeState::ACTOR_VIDEO_TIME;
	this->endSeeBackElapsedTime = USER_VIDEO_TIME + GazeState::ACTOR_VIDEO_TIME * 2;

	this->gazeRow = gazeRow;
	this->gazeCol = gazeCol;
	this->lockStartTime = std::chrono::high_resolution_clock::now();

	// Use the newest imageSeqence in director to create video.
	SetWaveVideo(gazeRow, gazeCol, std::make_shared<VideoClip>(director->GetUserImageSequenceRecords()->back(), USER_VIDEO_TIME, true, true));
}

GazeLockState::~GazeLockState()
{
}

void GazeLockState::Update()
{
	GazeState::Update();

	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lockStartTime).count();

	if (!isStartSeeGaze && elapsedTime > startSeeGazeElapsedTime) {
		SetAllSeeGazeVideo();
		isStartSeeGaze = true;
	}
	else if (!isStartSeeBack && elapsedTime > startSeeBackElapsedTime) {
		std::shared_ptr<Video> newVideo = GetActorDirectionVideo(gazeRow, gazeCol, ActorVideoSet::NEUTRAL, true, true);
		SetWaveVideo(gazeRow, gazeCol, newVideo);
		isStartSeeBack = true;
	}
	else if (elapsedTime > endSeeBackElapsedTime) {
		// Before leaving this state, reset all grid to NeutralVideo.
		SetNeutralVideo();
		director->SetInteractionState(std::make_shared<GazeWanderState>(director, GazeState::startTime));
	}
}

std::string GazeLockState::ToString()
{
	return "GazeLockState";
}

void GazeLockState::SetWaveVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> waveVideo = std::make_shared<WaveTransitionalVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		WAVE_TIME);

	director->GetVideoGrid()->SetChild(waveVideo, row, col);
}

void GazeLockState::SetAllSeeGazeVideo()
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			// Ignore gaze grid.
			if (row == gazeRow && col == gazeCol)
				continue;

			int direction = ActorVideoSet::GetDirectionIndex(row, col, gazeRow, gazeCol);
			std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, direction, true, true);
			SetBlendingVideo(row, col, newVideo);
		}
	}
}

void GazeLockState::SetNeutralVideo()
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			// Gaze grid has already been set to Neutral, so ignore it.
			if (row == gazeRow && col == gazeCol)
				continue;

			std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
			SetBlendingVideo(row, col, newVideo);
		}
	}
}
