/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef VIEW_STATE_H
#define VIEW_STATE_H

#include "AnimationState.h"
#include "Director.h"

/*!
 * \brief
 * Prelude animation state.
 * 
 * Prelude repeats flip animations from baby, girl to elder.
 */
class ViewState: public AnimationState
{
public:
	ViewState(): currentUserNum(0), firstcome(true) {
		int k[] = {15, 16, 17, 26, 27, 28, 37, 38, 39};
		for (int i=0; i<9; i++) {
			m[i] = k[i];
		}
	}

	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

private:
	bool firstcome;
	int currentUserNum;
	int m[9];
};

#endif // PRELUDE_STATE_H