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
#include "VideoRenderer.h"
#include "Director.h"
#include "PreludeWaveState.h"
#include "BuddhaAnimatedVideo.h"


class PreludeBuddhaState : public PreludeState
{
public:
	PreludeBuddhaState(Director *director, int buddhaStateCount = 0)
		: PreludeState(director)
	{
		this->buddhaStateCount = buddhaStateCount + 1;

		
		// TODO: layout
		std::shared_ptr<BuddhaAnimatedVideo> buddhaAnimatedVideo = std::make_shared<BuddhaAnimatedVideo>(director->GetVideoGrid(), PRELUDE_BUDDHA_STATE_TIME, 100, 100);
		VideoRenderer::GetInstance()->SetVideo(buddhaAnimatedVideo);
	}

	virtual ~PreludeBuddhaState()
	{}

	virtual void Update() override;

	virtual std::string ToString() override
	{
		return "PreludeBuddhaState";
	}

private:

	const float PRELUDE_BUDDHA_STATE_TIME = 10.0f;
	const int MAX_BUDDHA_STATE_COUNT = 5;

	int buddhaStateCount;

};

#endif // !_PRELUDE_BUDDHA_STATE_H
