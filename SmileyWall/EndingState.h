/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef ENDING_STATE_H
#define ENDING_STATE_H

#include "AnimationState.h"

typedef enum EndingStep{FADE_OUT_ALL, USER_SMILE, USER_ANI, EXPOSURE, SHOW_TEXT, END} EndingStep;

/*!
 * \brief
 * Ending animation state.
 * 
 * ??
 */
class EndingState: public AnimationState
{
public:
	EndingState(): duration(0.0f), endingStep(FADE_OUT_ALL) {
		for (int i=0; i<44; i++) {
			isUserAniStart[i] = false;
		}
	}

	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

private:
	cv::Mat photo;
	std::vector<std::vector<cv::Mat>> photoVec;
	cv::RNG randNumGenerator;
	float duration;
	EndingStep endingStep;

	std::vector<int> selectIndex;
	std::vector<int> answer;
	std::vector<int> order;
	std::vector<int> textOrder;

	std::vector<bool> isUserAns;
	bool isUserAniStart[44];
};

#endif // ENDING_STATE_H