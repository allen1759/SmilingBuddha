/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTRO_NEUTRAL_STATE_H
#define _INTRO_NEUTRAL_STATE_H

#include "IntroState.h"

#include <cstdlib>

#include "IntroSeeEachState.h"

class IntroNeutralState : public IntroState
{
public:
	IntroNeutralState(Director *director);

	~IntroNeutralState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:

	// Probability to create SeeEach animation. 10 means 100%.
	const int PROBABILITY_DENOMINATOR = 10;
	const int ANIMATION_PROBABILITY = 8;

	std::chrono::milliseconds videoDuration;
};


#endif // !_INTRO_NEUTRAL_STATE_H
