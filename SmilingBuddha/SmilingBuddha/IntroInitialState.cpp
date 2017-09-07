/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroInitialState.h"

#include "Setting.h"
#include "Director.h"
#include "IntroNeutralState.h"

IntroInitialState::IntroInitialState(Director *director)
	: IntroState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol())
{
	director->SetSeeEachSmileProcessorStrategy(INTRO_STATE_TIME);
	director->StartInteraction();

	InitializeVideoGrid();
}

IntroInitialState::~IntroInitialState()
{
}

void IntroInitialState::InitializeVideoGrid()
{
	for (int row = 0; row < ROW_COUNT; ++row) {
		for (int col = 0; col < COL_COUNT; ++col) {
			if (Setting::GetInstance()->IsInIntroStateGrid(row, col)) {
				std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
				director->GetVideoGrid()->SetChild(newVideo, row, col);
			}
			else
				director->GetVideoGrid()->SetChild(NULL, row, col);
		}
	}
}

std::string IntroInitialState::ToString()
{
	return "IntroInitialState";
}

void IntroInitialState::Update()
{
	director->SetInteractionState(std::make_shared<IntroNeutralState>(director));
	return;
}
