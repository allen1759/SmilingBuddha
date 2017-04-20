#pragma once
#ifndef ADD_ANIMATION_H
#define ADD_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

class AddAnimation : public Animation
{
public:
	AddAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg): 
	  Animation(startImg, endImg), rate(0.025f), alpha (0.0f) {}

	  AddAnimation(const CvArr *startImg, const CvArr *endImg): 
	  Animation(startImg, endImg), rate(0.025f), alpha (0.0f) {}

	  void play(Picture &picture);

	  inline void setFace(const cv::Mat& sf) {
		  smileFace = sf;
	  }

private:
	float alpha;
	float rate;
	cv::Mat smileFace;
};

#endif // ADD_ANIMATION_H