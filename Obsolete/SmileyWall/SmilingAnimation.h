/*!
 *\author Kai-Chih Huang
 * 
 *\copyright Copyright (c) 2012 by < Kai-Chih Huang/ NTU EE>
 */
#pragma once
#ifndef SmilingAnimation_H
#define SmilingAnimation_H

#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "ImagesPool.h"
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
//#include "morphing.h"
#include <stdio.h>


class SmilingAnimation : public Animation 

{
public:
	SmilingAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg):
	  Animation(startImg, endImg),counter(0), beta(0.6f),rate(0.005f),speedd(3), alpha (1.0f),fb(0),ori_light(1),sourceImage(startImg.getMat()){}
	SmilingAnimation(const CvArr *startImg, const CvArr *endImg):
	  Animation(startImg, endImg),counter(0), rate(0.005f), beta(0.6f),speedd(3),alpha (1.0f),fb(0),ori_light(1),sourceImage(cv::cvarrToMat(startImg)){}
	void play(Picture &picture);
	void setPicNo(int p) {picno = p;}
	void setType(int p) {type = p;}
	void setImagesPool(ImagesPool *ip) {mp = ip;}
	void setStartTime(clock_t t) {starttime = t;}
	int fb;
	double ori_light;
	int flag;
	
private:
	void display(Picture &picture);
private:
	int counter;
	int picno;
	cv::Mat sourceImage;
	clock_t starttime;
	ImagesPool *mp;
	float alpha;
	float beta;
	float rate;	
	int type;
	int speedd;
	
};

#endif