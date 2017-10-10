/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_BUDDHA_STATE_H
#define _PRELUDE_BUDDHA_STATE_H

#include "PreludeState.h"

#include <chrono>

//cpp
#include <cstdlib>
#include "Setting.h"
#include "VideoRenderer.h"
#include "Director.h"
#include "PreludeWaveState.h"
#include "PreludeEndingState.h"
#include "FadeAnimationVideo.h"
#include "BuddhaAnimatedVideo.h"

#include "AudioPool.h"
#include "AudioPlayer.h"

class PreludeBuddhaState : public PreludeState
{
public:
	PreludeBuddhaState(Director *director, int buddhaStateCount = 0)
		: PreludeState(director)
	{
		this->buddhaStateCount = buddhaStateCount + 1;

		// Select random grid.
		int randomRow = rand() % Setting::GetInstance()->GetRow();
		int randomCol = rand() % Setting::GetInstance()->GetCol();
		cv::Point buddhaPoint = Setting::GetInstance()->GetForeheadPositionOfGrid(randomRow, randomCol);
		
		buddhaAnimatedVideo = std::make_shared<BuddhaAnimatedVideo>(
			director->GetVideoGrid(), PRELUDE_BUDDHA_STATE_TIME, buddhaPoint.x, buddhaPoint.y);
		VideoRenderer::GetInstance()->SetVideo(buddhaAnimatedVideo);

		AudioPlayer::GetInstance()->PlayAudio(AudioPool::GetInstance()->GetBellAudio());
	}

	virtual ~PreludeBuddhaState()
	{
	}

	virtual void Update() override;

	virtual std::string ToString() override
	{
		return "PreludeBuddhaState";
	}

	virtual void OnUserDetect() override
	{
		// Set Black Fade Animation Video.
		std::shared_ptr<Video> fadeVideo = std::make_shared<FadeAnimationVideo>(
			buddhaAnimatedVideo,
			PreludeState::PRELUDE_ENDING_STATE_TIME, 0, 0, 0);

		VideoRenderer::GetInstance()->SetVideo(fadeVideo);

		director->SetInteractionState(std::make_shared<PreludeEndingState>(director));
	}

private:

	const float PRELUDE_BUDDHA_STATE_TIME = 10.0f;
	const int MAX_BUDDHA_STATE_COUNT = 5;

	int buddhaStateCount;

	std::shared_ptr<BuddhaAnimatedVideo> buddhaAnimatedVideo;
};

#endif // !_PRELUDE_BUDDHA_STATE_H
