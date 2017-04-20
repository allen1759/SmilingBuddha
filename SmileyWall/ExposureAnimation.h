#pragma once
#ifndef EXPOSURE_ANIMATION_H
#define EXPOSURE_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

typedef enum ExposureAnimationStyle{pic2exp, exp2pic} ExposureAnimationStyle;


class ExposureAnimation : public Animation
{
public:
	ExposureAnimation(const cv::Mat &startImg, const cv::Mat &endImg):
	  Animation(startImg, endImg), exposureStyle(pic2exp), scalar(0), speed(1), bound(255) {}

	ExposureAnimation(const CvArr *startImg, const CvArr *endImg):
	  Animation(startImg, endImg), exposureStyle(pic2exp), scalar(0), speed(1), bound(255) {}

	void play(Picture &picture);

	void setProperties(const ExposureAnimationStyle& newExposureStyle, 
		const double &newSpeed = 1, const double newScalar = -1, const int newBound = -1)
	{			
		exposureStyle = newExposureStyle;
		speed = newSpeed;
		if (newScalar == -1) 
			scalar = (exposureStyle == pic2exp)? 0 : 255;
		else
			scalar = newScalar;

		if (newBound == -1) 
			bound = (exposureStyle == pic2exp)? 255 : 0;
		else
			bound = newBound;
	}
	
private:
	ExposureAnimationStyle exposureStyle;
	double scalar;
	double speed;
	int bound;
};

#endif // EXPOSURE_ANIMATION_H