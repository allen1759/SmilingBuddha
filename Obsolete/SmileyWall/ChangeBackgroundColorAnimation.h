#pragma once
#ifndef GHANGEBC_ANIMATION_H
#define GHANGEBC_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class ChangeBackgroundColorAnimation : public Animation
{
public:
	ChangeBackgroundColorAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg): 
	  Animation(startImg, endImg), color(255) {}

	  ChangeBackgroundColorAnimation(const CvArr *startImg, const CvArr *endImg): 
	  Animation(startImg, endImg), color(255) {}

	  void play(Picture &picture);

	  void setProperty (int setColor) {
		  color = setColor;
	  }

private:
	int color;
};

#endif // GHANGEBC_ANIMATION_H