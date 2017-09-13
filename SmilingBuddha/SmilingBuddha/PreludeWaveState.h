/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_WAVE_STATE_H
#define _PRELUDE_WAVE_STATE_H

#include "PreludeState.h"

#include "VideoPool.h"

// cpp
#include <cstdlib>
#include "Director.h"
#include "VideoClip.h"
#include "WaveTransitionalVideo.h"
#include "PreludeBuddhaState.h"

class PreludeWaveState : public PreludeState
{
public:
	PreludeWaveState(Director *director, bool isFirstWave = true)
		: PreludeState(director)
	{
		this->videoPool = VideoPool::GetInstance();
		this->buddhaVideo = std::make_shared<VideoClip>(videoPool->GetNextBuddhaVideo(), 10.0, false, false);

		this->isFirstWave = isFirstWave;
		this->nextAppearTime = 0.0f;
	}

	virtual ~PreludeWaveState()
	{}

	static std::shared_ptr<PreludeWaveState> CreateRandomWaveState(Director *director, bool isFirstWave = true);

	virtual void Update() override;

protected:
	virtual void AppeartNext() = 0;

	virtual void UpdateAppearTime() = 0;

	void SetWaveVideo(int row, int col, std::shared_ptr<Video> newVideo)
	{
		std::shared_ptr<Video> waveVideo = std::make_shared<WaveTransitionalVideo>(
			director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
			newVideo,
			WAVE_TIME);

		director->GetVideoGrid()->SetChild(waveVideo, row, col);
	}

	const float PRELUDE_WAVE_STATE_TIME = 15.0f;
	const float ALL_APPEAR_TIME = 4.0f;
	const float WAVE_TIME = 3.0f;
	static const int PRELUDE_WAVE_STATE_COUNT = 5;

	VideoPool *videoPool;

	// TODO: replace by video frame
	std::shared_ptr<Video> buddhaVideo;

	bool isFirstWave;
	float nextAppearTime;
};

#endif // !_PRELUDE_WAVE_STATE_H
