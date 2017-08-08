/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef MIRROR_STATE_H
#define MIRROR_STATE_H

#include "AnimationState.h"

typedef enum ShowImageStep{SHOW_CLEAN, SHOW_LEFT, SHOW_RIGHT, SHOW_LEFT_FLIP, SHOW_WAIT} ShowImageStep;


/*!
 * \brief
 * Mirror animation state.
 * 
 * You see your mirror image and two laughing images
 around your mirror image in ZhengMeiXiang.
 */
class MirrorState: public AnimationState
{
public:
	MirrorState(): duration(0.0f), showImageStep(SHOW_CLEAN) {
		index.assign(3, 0);
		isRandomed.assign(3, false);
	}
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

private:
	float duration;
	ShowImageStep showImageStep;
	std::vector<int> index;
	std::vector<bool> isRandomed;

	cv::Mat photo;
	std::vector<cv::Mat> photoVec;
};

#endif // MIRROR_STATE_H