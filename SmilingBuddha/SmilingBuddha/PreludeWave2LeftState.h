/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_WAVE_2_LEFT_STATE_H
#define _PRELUDE_WAVE_2_LEFT_STATE_H

#include "PreludeWaveState.h"

//cpp
#include "Setting.h"

class PreludeWave2LeftState : public PreludeWaveState
{
public:
	PreludeWave2LeftState(Director *director, bool isFirstWave)
		: PreludeWaveState(director, isFirstWave),
		  APPEAR_TIME_FOR_EACH(PreludeWaveState::ALL_APPEAR_TIME / Setting::GetInstance()->GetCol())
	{
		this->currentColumn = Setting::GetInstance()->GetCol() - 1;
	}

	virtual ~PreludeWave2LeftState()
	{}

	virtual std::string ToString() override
	{
		return "PreludeWave2LeftState";
	}

private:
	virtual void AppeartNext() override
	{
		for (int row = 0; row < Setting::GetInstance()->GetRow(); ++row) {
			if (PreludeWaveState::isFirstWave) {
				PreludeWaveState::SetWaveVideo(row, currentColumn, PreludeWaveState::buddhaVideo);
			}
			else {
				std::shared_ptr<Video> smileVideo = std::make_shared<VideoClip>(videoPool->GetSmileVideoList(row, currentColumn), PreludeState::USER_VIDEO_TIME, true, true);
				PreludeWaveState::SetWaveVideo(row, currentColumn, smileVideo);
			}
		}
		currentColumn--;

		UpdateAppearTime();
	}

	virtual void UpdateAppearTime() override
	{
		if (currentColumn < 0)
			PreludeWaveState::nextAppearTime = PreludeWaveState::PRELUDE_WAVE_STATE_TIME;
		else
			PreludeWaveState::nextAppearTime += APPEAR_TIME_FOR_EACH;
	}

	const float APPEAR_TIME_FOR_EACH;
	int currentColumn;
};

#endif // !_PRELUDE_WAVE_2_LEFT_STATE_H


