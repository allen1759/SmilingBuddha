/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _PRELUDE_ENDING_STATE_H
#define _PRELUDE_ENDING_STATE_H

#include "PreludeState.h"

#include <chrono>

#include "IntroInitialState.h"
class PreludeEndingState : public PreludeState
{
public:
	PreludeEndingState(Director *director);

	virtual ~PreludeEndingState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
};

#endif // !_PRELUDE_ENDING_STATE_H
