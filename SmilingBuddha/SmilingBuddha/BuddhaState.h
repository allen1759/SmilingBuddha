/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _BUDDHA_STATE_H
#define _BUDDHA_STATE_H

#include "EndingState.h"

#include "Video.h"

class BuddhaState : public EndingState
{
public:
	BuddhaState(Director *director);

	virtual ~BuddhaState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	virtual void SetTransition(int row, int col);

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	const float BUDDHA_STATE_TIME = 18.0f;
	// TODO: change to videoFrame
	const float BUDDHA_VIDEO_TIME = 10.0f;
	const float WAVE_TIME = 3.0f;

	std::shared_ptr<Video> buddhaVideo;
	int currentDistance;

	float nextAppearElapsedTime;
};

#endif // !_BUDDHA_STATE_H
