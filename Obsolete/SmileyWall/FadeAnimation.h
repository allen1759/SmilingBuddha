#pragma once
#ifndef FADE_ANIMATION_H
#define FADE_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

typedef enum FadeAnimationStyle{FADE_IN, FADE_OUT, FADE_TO} FadeAnimationStyle;

class FadeAnimation : public Animation
{
public:
	FadeAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg): 
	  Animation(startImg, endImg), fadeStyle(FADE_IN), rate(0.025f), alpha (0.0f) {}
	
	FadeAnimation(const CvArr *startImg, const CvArr *endImg): 
	  Animation(startImg, endImg), fadeStyle(FADE_IN), rate(0.025f), alpha (0.0f) {}

	void play(Picture &picture);

	inline void setProperties(const FadeAnimationStyle& newFadeStyle,
		const float &newRate = 0.025f) 
	{			
		fadeStyle = newFadeStyle;
		alpha = (fadeStyle == FADE_IN)? 0.0f : 1.0f;
		rate = newRate;
	}

private:
	FadeAnimationStyle fadeStyle;
	float alpha;
	float rate;
};

#endif // FADE_ANIMATION_H