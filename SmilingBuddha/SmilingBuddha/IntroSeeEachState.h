/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTRO_SEE_EACH_STATE_H
#define _INTRO_SEE_EACH_STATE_H

#include "IntroState.h"

class IntroSeeEachState : public IntroState
{
public:
	IntroSeeEachState(Director *director);

	virtual ~IntroSeeEachState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	/**
	* Random choose two grid in IntroStateGrid. Then set see each other animation.
	*
	*/
	void SetSeeEachVideo();

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	const int ROW_CENTER;
	const int COL_CENTER;

	const float BLENDING_TIME = 0.5f;

	int lastFromRow, lastFromCol;
	int lastAtRow, lastAtCol;

	float endingElapsedTime;
};

#endif // !_INTRO_SEE_EACH_STATE_H
