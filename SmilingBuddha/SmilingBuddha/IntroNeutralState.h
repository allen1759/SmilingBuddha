/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTRO_NEUTRAL_STATE_H
#define _INTRO_NEUTRAL_STATE_H

#include "IntroState.h"

class IntroNeutralState : public IntroState
{
public:
	IntroNeutralState(Director *director);

	virtual ~IntroNeutralState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:

	float endingElapsedTime;
};


#endif // !_INTRO_NEUTRAL_STATE_H
