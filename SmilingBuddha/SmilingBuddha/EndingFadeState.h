/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ENDING_FADE_STATE_H
#define _ENDING_FADE_STATE_H

#include "EndingState.h"

#include "Video.h"

class EndingFadeState : public EndingState
{
public:
	EndingFadeState(Director *director);

	virtual ~EndingFadeState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	virtual void SetTransition(int row, int col) override;


	const float ENDING_FADE_STATE_TIME = 20.0f;
	// TODO: change to videoFrame
	const float FADE_VIDEO_TIME = 10.0f;
	const float FADE_ANIMATION_TIME = 3.0f;

	std::shared_ptr<Video> endingFadeVideo;
	int currentDistance;
	float nextAppearElapsedTime;
};


#endif // !_ENDING_FADE_STATE_H

#include "EndingState.h"

