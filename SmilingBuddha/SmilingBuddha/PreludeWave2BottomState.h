/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_WAVE_2_BOTTOM_STATE_H
#define _PRELUDE_WAVE_2_BOTTOM_STATE_H

#include "PreludeWaveState.h"

//cpp
#include "Setting.h"

class PreludeWave2BottomState : public PreludeWaveState
{
public:
	PreludeWave2BottomState(Director *director, bool isFirstWave)
	: PreludeWaveState(director, isFirstWave),
		APPEAR_TIME_FOR_EACH(PreludeWaveState::ALL_APPEAR_TIME / Setting::GetInstance()->GetRow())
	{
		this->currentRow = 0;
	}

	virtual ~PreludeWave2BottomState()
	{}

	virtual std::string ToString() override
	{
		return "PreludeWave2BottomState";
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
		currentRow++;

		UpdateAppearTime();
	}

	virtual void UpdateAppearTime() override
	{
		if (currentRow >= Setting::GetInstance()->GetRow())
			PreludeWaveState::nextAppearTime = PreludeWaveState::PRELUDE_WAVE_STATE_TIME;
		else
			PreludeWaveState::nextAppearTime += APPEAR_TIME_FOR_EACH;
	}

	const float APPEAR_TIME_FOR_EACH;
	int currentRow;
};

#endif // !_PRELUDE_WAVE_2_BOTTOM_STATE_H
