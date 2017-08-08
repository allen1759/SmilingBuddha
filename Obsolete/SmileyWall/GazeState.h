
/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef GAZE_STATE_H
#define GAZE_STATE_H

#include "AnimationState.h"
#include "ImagesPool.h"
#include <time.h>

typedef enum GazeStep{GAZE_INITIAL, FIRSTLINE, SECONDLINE, ALL} GazeStep;

const int firstLineIndex[] = {0, 8, 9, 17, 18, 26, 27, 35};
const int secondLineIndex[] = {1, 7, 10, 16, 19, 25, 28, 34};

/*!
 * \brief
 * Interactive animation state.
 * 
 * Show your photo and interact with ZhengMeiXiang.
 */
class GazeState: public AnimationState
{
public:
	GazeState(): eyePosIndex(22), stateStartTime(0), gazeStep(GAZE_INITIAL), currentEyePosIndex(22), addFlag(false),
		isEyeStayMoreThenThreshold(0), starttime(0), first(1), waitOSC(false), userNo(1) {
			
	}
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

	

private:
	int eyePosIndex;
	double stateStartTime;
	double currentTime;
	GazeStep gazeStep;
	int currentEyePosIndex;
	bool addFlag;
	double starttime;
	int attpt;
	int isEyeStayMoreThenThreshold;
	int first; //1: new or last time out of threshold/8-neighbor
	int isDisplay[36];
	bool waitOSC;
	double waitOSCstartTime;
	int userNo;

	void SetCENTERAnimation( Picture &picture, ImagesPool * imagesPool, int i, Director * director );
};

#endif // GAZE_STATE_H