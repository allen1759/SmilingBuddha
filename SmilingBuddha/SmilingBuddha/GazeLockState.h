/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_LOCK_STATE_H
#define _GAZE_LOCK_STATE_H

#include "GazeState.h"


//cpp
#include "Setting.h"
#include "Director.h"
#include "BroadcastState.h"
#include "VideoClip.h"
#include "BlendingTransitionVideo.h"
#include "WaveTransitionalVideo.h"

class GazeLockState : public GazeState
{
public:
	GazeLockState(Director *director, std::chrono::high_resolution_clock::time_point startTime, int gazeRow, int gazeCol)
		: GazeState(director, startTime),
		  ROW_COUNT(Setting::GetInstance()->GetRow()),
		  COL_COUNT(Setting::GetInstance()->GetCol())
	{
		this->isStartSeeGaze = false;
		this->isStartSeeBack = false;

		this->gazeRow = gazeRow;
		this->gazeCol = gazeCol;
		this->lockStartTime = std::chrono::high_resolution_clock::now();

		SetWaveVideo(gazeRow, gazeCol, std::make_shared<VideoClip>(director->GetUserImageSequenceRecords()->back(), USER_VIDEO_TIME, true, true));
	}

	virtual ~GazeLockState()
	{
	}

	virtual void Update() override;

	virtual std::string ToString() override
	{
		return "GazeLockState";
	}

private:
	void SetAllSeeGazeVideo()
	{
		for (int row = 0; row < ROW_COUNT; ++row) {
			for (int col = 0; col < COL_COUNT; ++col) {
				if (row == gazeRow && col == gazeCol)
					continue;
				int direction = ActorVideoSet::GetDirectionIndex(row, col, gazeRow, gazeCol);
				std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, direction, true, true);
				SetBlendingVideo(row, col, newVideo);
			}
		}
	}

	void SetNeutralVideo()
	{
		for (int row = 0; row < ROW_COUNT; ++row) {
			for (int col = 0; col < COL_COUNT; ++col) {
				if (row == gazeRow && col == gazeCol)
					continue;

				std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
				SetBlendingVideo(row, col, newVideo);
			}
		}
	}

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
	{
		std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
			director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
			newVideo,
			BLENDING_TIME);

		director->GetVideoGrid()->SetChild(blendingVideo, row, col);
	}
	
	void SetWaveVideo(int row, int col, std::shared_ptr<Video> newVideo)
	{
		std::shared_ptr<Video> waveVideo = std::make_shared<WaveTransitionalVideo>(
			director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
			newVideo,
			APPEAR_TIME);

		director->GetVideoGrid()->SetChild(waveVideo, row, col);
	}


	const int ROW_COUNT;
	const int COL_COUNT;

	const float USER_VIDEO_TIME = 3.0;
	const float APPEAR_TIME = 3.0f;
	const float SEE_GAZE_VIDEO_TIME = 3.0f;
	const float BLENDING_TIME = 0.5f;


	bool isStartSeeGaze;
	bool isStartSeeBack;
	float startSeeGazeElapsedTime = USER_VIDEO_TIME;
	float startSeeBackElapsedTime = USER_VIDEO_TIME + SEE_GAZE_VIDEO_TIME;
	float endSeeBackElapsedTime = USER_VIDEO_TIME + SEE_GAZE_VIDEO_TIME * 2;

	int gazeRow;
	int gazeCol;
	std::chrono::high_resolution_clock::time_point lockStartTime;
};

#endif // !_GAZE_LOCK_STATE_H
