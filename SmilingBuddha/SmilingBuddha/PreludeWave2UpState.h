/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_WAVE_2_UP_STATE_H
#define _PRELUDE_WZVE_2_UP_STATE_H

#include "PreludeWaveState.h"

//cpp
#include "Setting.h"

class PreludeWave2UpState : public PreludeWaveState
{
public:
	PreludeWave2UpState(Director *director, bool isFirstWave)
		: PreludeWaveState(director, isFirstWave),
		APPEAR_TIME_FOR_EACH(PreludeWaveState::ALL_APPEAR_TIME / Setting::GetInstance()->GetRow())
	{
		this->currentRow = Setting::GetInstance()->GetRow() - 1;
	}

	virtual ~PreludeWave2UpState()
	{}

	virtual std::string ToString() override
	{
		return "PreludeWave2UpState";
	}

private:
	virtual void AppeartNext() override
	{
		for (int col = 0; col < Setting::GetInstance()->GetCol(); ++col) {
			if (PreludeWaveState::isFirstWave) {
				PreludeWaveState::SetWaveVideo(currentRow, col, PreludeWaveState::buddhaVideo);
			}
			else {
				std::shared_ptr<Video> smileVideo = std::make_shared<VideoClip>(videoPool->GetSmileVideoList(currentRow, col), PreludeState::USER_VIDEO_TIME, true, true);
				PreludeWaveState::SetWaveVideo(currentRow, col, smileVideo);
			}
		}
		currentRow--;

		UpdateAppearTime();
	}

	virtual void UpdateAppearTime() override
	{
		if (currentRow < 0)
			PreludeWaveState::nextAppearTime = PreludeWaveState::PRELUDE_WAVE_STATE_TIME;
		else
			PreludeWaveState::nextAppearTime += APPEAR_TIME_FOR_EACH;
	}

	const float APPEAR_TIME_FOR_EACH;
	int currentRow;
};

#endif // !_PRELUDE_WAVE_2_UP_STATE_H
