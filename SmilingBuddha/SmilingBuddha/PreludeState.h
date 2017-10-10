/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_STATE_H
#define _PRELUDE_STATE_H

#include "InteractionState.h"

#include <chrono>

class PreludeState : public InteractionState
{
public:
	PreludeState(Director *director);

	virtual ~PreludeState();

	virtual void OnLayoutTrigger() override;

protected:

	const float USER_VIDEO_TIME = 3.0f;
	const float PRELUDE_ENDING_STATE_TIME = 1.0f;

	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_PRELUDE_STATE_H
