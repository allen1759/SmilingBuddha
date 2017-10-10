/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_WAVE_2_RANDOM_STATE_H
#define _PRELUDE_WAVE_2_RANDOM_STATE_H

#include "PreludeWaveState.h"

//cpp
#include <algorithm>
#include "Setting.h"

class PreludeWave2RandomState : public PreludeWaveState
{
public:
	PreludeWave2RandomState(Director *director, bool isFirstWave)
		: PreludeWaveState(director, isFirstWave),
		APPEAR_TIME_FOR_EACH(PreludeWaveState::ALL_APPEAR_TIME / Setting::GetInstance()->GetWindowCount())
	{
		for (int i = 0; i < Setting::GetInstance()->GetWindowCount(); ++i)
			this->indices.push_back(i);
		std::random_shuffle(indices.begin(), indices.end());
		this->currentIndex = 0;
	}

	virtual ~PreludeWave2RandomState()
	{}

	virtual std::string ToString() override
	{
		return "PreludeWave2RandomState";
	}

private:
	virtual void AppeartNext() override
	{
		int row = indices[currentIndex] / Setting::GetInstance()->GetCol();
		int col = indices[currentIndex] % Setting::GetInstance()->GetCol();

		if (PreludeWaveState::isFirstWave) {
			PreludeWaveState::SetWaveVideo(row, col, PreludeWaveState::buddhaVideo);
		}
		else {
			std::shared_ptr<Video> smileVideo = std::make_shared<VideoClip>(videoPool->GetSmileVideoList(row, col), PreludeState::USER_VIDEO_TIME, true, true);
			PreludeWaveState::SetWaveVideo(row, col, smileVideo);
		}
		currentIndex++;

		UpdateAppearTime();
	}

	virtual void UpdateAppearTime() override
	{
		if (currentIndex >= indices.size())
			PreludeWaveState::nextAppearTime = PreludeWaveState::PRELUDE_WAVE_STATE_TIME;
		else
			PreludeWaveState::nextAppearTime += APPEAR_TIME_FOR_EACH;
	}

	const float APPEAR_TIME_FOR_EACH;
	std::vector<int> indices;
	int currentIndex;
};

#endif // !_PRELUDE_WAVE_2_RANDOM_STATE_H
