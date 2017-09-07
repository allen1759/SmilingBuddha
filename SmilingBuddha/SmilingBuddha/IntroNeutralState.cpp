/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroNeutralState.h"

#include "Director.h"
#include "IntroSeeEachState.h"
#include "SmileState.h"

IntroNeutralState::IntroNeutralState(Director *director)
	: IntroState(director)
{
	this->endingElapsedTime = std::chrono::milliseconds(static_cast<int>(IntroState::VIDEO_TIME * 2 * 1000));
}

IntroNeutralState::~IntroNeutralState()
{
}

void IntroNeutralState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - IntroState::startTime);

	if (elapsedTime > endingElapsedTime) {
		if (IntroState::switchToSmileState) {
			director->SetInteractionState(std::make_shared<SmileState>(director, userImages));
			return;
		}
		else {
			director->SetInteractionState(std::make_shared<IntroSeeEachState>(director));
			return;
		}
	}
}

std::string IntroNeutralState::ToString()
{
	return "IntroNeutralState";
}
