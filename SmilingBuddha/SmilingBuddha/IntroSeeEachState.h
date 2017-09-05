/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTRO_SEE_EACH_STATE_H
#define _INTRO_SEE_EACH_STATE_H

#include "IntroState.h"

#include "IntroNeutralState.h"

class IntroSeeEachState : public IntroState
{
public:
	IntroSeeEachState(Director *director);

	~IntroSeeEachState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	void SetSmileAnimation();


	// With 80% probability to create SeeEach animation.
	const int ANIMATION_PROBABILITY = 8;

	int lastFromRow, lastFromCol;
	int lastAtRow, lastAtCol;

	std::chrono::milliseconds animationDuration;
};

#endif // !_INTRO_SEE_EACH_STATE_H
