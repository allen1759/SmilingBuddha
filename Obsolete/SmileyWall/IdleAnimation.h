/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef IDLE_ANIMATION_H
#define IDLE_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

/*!
 * \brief
 * Idle animation.

 It it looked as a initial animation.
 */
class IdleAnimation : public Animation
{
public:
	IdleAnimation(): Animation() {}
	IdleAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg): 
		Animation(startImg, endImg) {}
	IdleAnimation(const CvArr *startImg, const CvArr *endImg): 
		Animation(startImg, endImg) {}
	void play(Picture &picture) {ended = true;}
};

#endif // IDLE_ANIMATION_H