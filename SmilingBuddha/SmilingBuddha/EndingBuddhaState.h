/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ENDING_BUDDHA_STATE_H
#define _ENDING_BUDDHA_STATE_H

#include "EndingState.h"

#include "Video.h"

class EndingBuddhaState : public EndingState
{
public:
	EndingBuddhaState(Director *director);

	virtual ~EndingBuddhaState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	virtual void SetTransition(int row, int col);


	const float BUDDHA_STATE_TIME = 18.0f;
	// TODO: change to videoFrame
	const float BUDDHA_VIDEO_TIME = 10.0f;
	const float WAVE_TIME = 3.0f;

	std::shared_ptr<Video> buddhaVideo;
	int currentDistance;

	float nextAppearElapsedTime;
};

#endif // !_ENDING_BUDDHA_STATE_H
