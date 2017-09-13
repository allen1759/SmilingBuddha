/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_INITIAL_STATE_H
#define _PRELUDE_INITIAL_STATE_H

#include "PreludeState.h"

#include "VideoPool.h"
//cpp
#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "PreludeBuddhaState.h"

class PreludeInitialState : public PreludeState
{
public:
	PreludeInitialState(Director *director)
		: PreludeState(director),
		  ROW_COUNT(Setting::GetInstance()->GetRow()),
		  COL_COUNT(Setting::GetInstance()->GetCol())
	{
		videoPool = VideoPool::GetInstance();

		InitializeSmileVideo();
	}

	virtual ~PreludeInitialState()
	{}

	virtual void Update() override
	{
		director->SetInteractionState(std::make_shared<PreludeBuddhaState>(director));
		return;
	}

	virtual std::string ToString() override
	{
		return "PreludeInitialState";
	}

private:
	void InitializeSmileVideo()
	{
		for (int row = 0; row < ROW_COUNT; ++row) {
			for (int col = 0; col < COL_COUNT; ++col) {
				std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(videoPool->GetSmileVideoList(row, col), PreludeState::USER_VIDEO_TIME, true, true);
				director->GetVideoGrid()->SetChild(
					newVideo, row, col);
			}
		}
	}


	const int ROW_COUNT;
	const int COL_COUNT;

	VideoPool *videoPool;
};

#endif // !_PRELUDE_INITIAL_STATE_H
