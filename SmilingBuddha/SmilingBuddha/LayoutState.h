/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _LAYOUT_STATE_H
#define _LAYOUT_STATE_H

#include "InteractionState.h"

#include "Setting.h"
#include "InputManager.h"
#include "VideoRenderer.h"
#include "Director.h"

// Video
#include "FadeAnimationVideo.h"

// State
#include "PreludeInitialState.h"

class LayoutState : public InteractionState
{
public:
	LayoutState(Director *director)
		: InteractionState(director),
		  layout(Setting::GetInstance()->GetLayout()),
		  ROW_COUNT(Setting::GetInstance()->GetRow()),
		  COL_COUNT(Setting::GetInstance()->GetCol())
	{
		currentRow = 0;
		currentCol = 0;

		VideoRenderer::GetInstance()->SetVideo(director->GetVideoGrid());

		std::shared_ptr<Video> video = director->GetVideoGrid()->GetChild(currentRow, currentCol);
		video = std::make_shared<FadeAnimationVideo>(video, 1.0, 255, 255, 255);
		director->GetVideoGrid()->SetChild(video, currentRow, currentCol);
	}

	virtual ~LayoutState()
	{
	}

	virtual void Update() override
	{
		int nextCol = currentCol;
		int nextRow = currentRow;

		// Select grid we want to adjust.
		if (InputManager::GetInstance()->GetKey('a')) {
			nextCol--;
			UpdateAnimation(nextRow, nextCol);
		}
		else if (InputManager::GetInstance()->GetKey('d')) {
			nextCol++;
			UpdateAnimation(nextRow, nextCol);
		}
		else if (InputManager::GetInstance()->GetKey('w')) {
			nextRow--;
			UpdateAnimation(nextRow, nextCol);
		}
		else if (InputManager::GetInstance()->GetKey('s')) {
			nextRow++;
			UpdateAnimation(nextRow, nextCol);
		}

		// Adjust position of seleted grid.
		cv::Point point = layout[currentRow * COL_COUNT + currentCol];

		if (InputManager::GetInstance()->GetKey('f')) {
			point.x--;
			Setting::GetInstance()->SetLayout(currentRow, currentCol, point);
		}
		else if (InputManager::GetInstance()->GetKey('h')) {
			point.x++;
			Setting::GetInstance()->SetLayout(currentRow, currentCol, point);
		}
		else if (InputManager::GetInstance()->GetKey('t')) {
			point.y--;
			Setting::GetInstance()->SetLayout(currentRow, currentCol, point);
		}
		else if (InputManager::GetInstance()->GetKey('g')) {
			point.y++;
			Setting::GetInstance()->SetLayout(currentRow, currentCol, point);
		}
	}

	virtual std::string ToString() override
	{
		return "LayoutState";
	}
	
	virtual void OnLayoutTrigger() override
	{
		Setting::GetInstance()->SaveLayout();
		director->SetInteractionState(std::make_shared<PreludeInitialState>(director));
	}

private:
	void UpdateAnimation(int nextRow, int nextCol)
	{
		// Check boundary.
		nextCol = std::max(0, std::min(nextCol, COL_COUNT - 1));
		nextRow = std::max(0, std::min(nextRow, ROW_COUNT - 1));

		std::shared_ptr<Video> video = director->GetVideoGrid()->GetChild(currentRow, currentCol);
		director->GetVideoGrid()->SetChild(video->GetVideo(), currentRow, currentCol);

		video = director->GetVideoGrid()->GetChild(nextRow, nextCol);
		video = std::make_shared<FadeAnimationVideo>(video, 1.0, 255, 255, 255);
		director->GetVideoGrid()->SetChild(video, nextRow, nextCol);
		
		currentRow = nextRow;
		currentCol = nextCol;
	}

	const int ROW_COUNT;
	const int COL_COUNT;

	int currentRow;
	int currentCol;
	const std::vector<cv::Point> &layout;
};

#endif // !_LAYOUT_STATE_H

