/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef LOOK_ANIMATION_H
#define LOOK_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>


typedef enum LookAnimationState{LOOK_1, LOOK_2, LOOK_END} LookAnimationState; //!< - LOOK_1: start image -> end image.
																			  //!< - LOOK_2: end imaeg -> start image.
																			  //!< - LOOK_END: the animation is end.
/*!
 * \brief
 * Look animation.
 * 
 * 3 steps:
	- Initialize a rotation axis and compute a rotation angle
	- Compute a rotation matrix
	- Perspective transformation
 * 
 */
class LookAnimation : public Animation 
{
public:
	LookAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg):
	  Animation(startImg, endImg), lookState(LOOK_1), photoNum(-1)
	  {
		  initWhatNeed();
	  }
	LookAnimation(const CvArr *startImg, const CvArr *endImg):
	  Animation(startImg, endImg), lookState(LOOK_1), photoNum(-1)
	  {
		  initWhatNeed();
	  }
	void play(Picture &picture);

	/*!
	*  \brief A setter for some properties about the look animation.
	*  \param orientation a look orientation
	   \param newSpeed a look speed.
	*/
	inline void setProperties(std::vector<cv::Mat> &specSet,
		const double &costime = 2.0, int playPhotoNum = -1, LookAnimationState setState = LOOK_1) 
	{
		photoSet = specSet;
		lookState = setState;
		initWhatNeed();
		runtime = costime;
		if (playPhotoNum == -1) {
			photoNum = static_cast<int>(photoSet.size());
		} else {
			photoNum = playPhotoNum;
		}
	}

	inline double getStartTime()
	{
		return startTime;
	}
	
private:
	void initWhatNeed();
	void handleLookState();
	void display(Picture &picture);

private:
	LookAnimationState lookState;
	std::vector<cv::Mat> photoSet;
	int firstPhotoNum;
	double runtime;
	double startTime;
	double currentTime;
	int photoNum;
};

#endif // LOOK_ANIMATION_H