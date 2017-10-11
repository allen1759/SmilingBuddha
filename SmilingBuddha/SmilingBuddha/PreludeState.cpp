/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "PreludeState.h"

#include "InputManager.h"
#include "Director.h"

// State
#include "LayoutState.h"
#include "RoiState.h"

PreludeState::PreludeState(Director * director)
	: InteractionState(director)
{
	this->startTime = std::chrono::high_resolution_clock::now();
}

PreludeState::~PreludeState()
{
}

void PreludeState::OnLayoutTrigger()
{
	director->SetInteractionState(std::make_shared<LayoutState>(director));
}

void PreludeState::OnRoiTrigger()
{
	director->SetInteractionState(std::make_shared<RoiState>(director));
}
