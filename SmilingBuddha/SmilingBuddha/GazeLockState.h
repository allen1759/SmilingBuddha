/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_LOCK_STATE_H
#define _GAZE_LOCK_STATE_H

#include "GazeState.h"


//cpp
#include "Director.h"
#include "BlendingTransitionVideo.h"

class GazeLockState : public GazeState
{
public:
	GazeLockState(Director *director, std::chrono::high_resolution_clock::time_point startTime, int gazeRow, int gazeCol);

	virtual ~GazeLockState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	// Set wave animation with given video.
	void SetWaveVideo(int row, int col, std::shared_ptr<Video> newVideo);

	// Set actors to look at gaze grid.
	void SetAllSeeGazeVideo();

	// Reset all actors except gaze grid to NeutralVideo.
	void SetNeutralVideo();

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
	{
		std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
			director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
			newVideo,
			BLENDING_TIME);

		director->GetVideoGrid()->SetChild(blendingVideo, row, col);
	}


	const float USER_VIDEO_TIME = 3.0;
	const float WAVE_TIME = 3.0f;
	const float BLENDING_TIME = 0.5f;

	bool isStartSeeGaze;
	bool isStartSeeBack;
	float startSeeGazeElapsedTime;
	float startSeeBackElapsedTime;
	float endSeeBackElapsedTime;

	int gazeRow;
	int gazeCol;
	std::chrono::high_resolution_clock::time_point lockStartTime;
};

#endif // !_GAZE_LOCK_STATE_H
