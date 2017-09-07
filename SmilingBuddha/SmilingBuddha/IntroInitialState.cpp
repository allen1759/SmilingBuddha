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
	//for (int i = 0; i < SQUARE_SIZE; ++i) {
	//	int row = CENTER_ROW + DIRECTION[i][1];
	//	int col = CENTER_COL + DIRECTION[i][0];
	//	if (col < 0 || col >= COL_COUNT)
	//		continue;
	//	if (row < 0 || row >= ROW_COUNT)
	//		continue;

	//	std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, ActorVideoSet::NEUTRAL, true, true);
	//	director->GetVideoGrid()->SetChild(newVideo, row, col);
	//}

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
