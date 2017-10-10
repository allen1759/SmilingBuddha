/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "PreludeEndingState.h"

#include "Director.h"

PreludeEndingState::PreludeEndingState(Director * director)
	: PreludeState(director)
{
}

PreludeEndingState::~PreludeEndingState()
{
}

void PreludeEndingState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - PreludeState::startTime).count();

	if (elapsedTime > PreludeState::PRELUDE_ENDING_STATE_TIME) {
		director->SetInteractionState(std::make_shared<IntroInitialState>(director));
		return;
	}
}

std::string PreludeEndingState::ToString()
{
	return "PreludeEndingState";
}
