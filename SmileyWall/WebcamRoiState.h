/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef WEBCAM_ROI_STATE_H
#define WEBCAM_ROI_STATE_H

#include "AnimationState.h"


/*!
 * \brief
 * Webcam animation state.
 * 
 * Show ROI image of webcam in center. You can modify the ROI of webcam image.
 */
class WebcamRoiState: public AnimationState
{
public:
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

private:
};

#endif // WEBCAM_ROI_STATE_H