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
#include "Director.h"
#include "PreludeWaveState.h"


#include  "FadeAnimationVideo.h"

class PreludeBuddhaState : public PreludeState
{
public:
	PreludeBuddhaState(Director *director, int buddhaStateCount = 0)
		: PreludeState(director)
	{
		this->buddhaStateCount = buddhaStateCount + 1;

		// TODO: add buddha animation
		director->GetVideoGrid()->SetChild(
			std::make_shared<FadeAnimationVideo>(director->GetVideoGrid()->GetChild(2, 4), 3.0f, 255, 0, 0),
			2, 4);
	}

	virtual ~PreludeBuddhaState()
	{}

	virtual void Update() override;

	virtual std::string ToString() override
	{
		return "PreludeBuddhaState";
	}

private:

	const float PRELUDE_BUDDHA_STATE_TIME = 1.0f;
	const int MAX_BUDDHA_STATE_COUNT = 5;

	int buddhaStateCount;

};

#endif // !_PRELUDE_BUDDHA_STATE_H
