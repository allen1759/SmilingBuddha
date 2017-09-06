/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroNeutralState.h"

#include <cstdlib>

#include "Director.h"
#include "SmileState.h"

IntroNeutralState::IntroNeutralState(Director *director)
	:IntroState(director)
{
	this->videoDuration = std::chrono::milliseconds(static_cast<int>(VIDEO_TIME * 1000));
}

IntroNeutralState::~IntroNeutralState()
{
}

void IntroNeutralState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

	if (delta > videoDuration) {
		if (switchToSmileState) {
			director->SetInteractionState(std::make_shared<SmileState>(director, userImages));
			return;
		}
		else if (rand() % PROBABILITY_DENOMINATOR < ANIMATION_PROBABILITY) {
			director->SetInteractionState(std::make_shared<IntroSeeEachState>(director));
			return;
		}
		else {
			// Restart NeutralState.
			startTime = std::chrono::high_resolution_clock::now();
		}
	}
}

std::string IntroNeutralState::ToString()
{
	return "IntroNeutralState";
}
