/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTRO_INITIAL_STATE_H
#define _INTRO_INITIAL_STATE_H

#include "IntroState.h"

class IntroInitialState : public IntroState
{
public:
	IntroInitialState(Director *director);

	virtual ~IntroInitialState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	/**
	* Set actors to Neutral video which in IntroStateGrid.
	*
	*/
	void InitializeVideoGrid();

	const int ROW_COUNT;
	const int COL_COUNT;
};

#endif // !_INTRO_INITIAL_STATE_H
