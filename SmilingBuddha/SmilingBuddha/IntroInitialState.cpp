/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroInitialState.h"

#include "Director.h"
#include "IntroNeutralState.h"

IntroInitialState::IntroInitialState(Director *director)
	:IntroState(director)
{
	InitializeVideoGrid();
	director->SetSeeEachSmileProcessorStrategy(INTRO_STATE_TIME);
	director->StartInteraction();
}

IntroInitialState::~IntroInitialState()
{
}

void IntroInitialState::InitializeVideoGrid()
{
	for (int i = 0; i < SQUARE_SIZE; ++i) {
		int row = centerRow + DIRECTION[i][1];
		int col = centerCol + DIRECTION[i][0];
		if (col < 0 || col >= colCount)
			continue;
		if (row < 0 || row >= rowCount)
			continue;

		std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
		director->GetVideoGrid()->SetChild(newVideo, row, col);
	}
}

std::string IntroInitialState::ToString()
{
	return "IntroInitialState";
}

void IntroInitialState::Update()
{
	this->director->SetInteractionState(std::make_shared<IntroNeutralState>(director));
}