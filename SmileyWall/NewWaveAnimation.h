#pragma once
#ifndef NEWWAVE_ANIMATION_H
#define NEWWAVE_ANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>
#include <vector>

class NewWaveAnimation : public Animation 
{
public:
	  NewWaveAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg):
	  Animation(startImg, endImg), sourceImage(endImg.getMat()), isAlmostWaveEnergyIsZero(false),
		  scale(1), speedUp(false), radius(20), damping(3), initEnergy(1800)
	  {
		  cv::Size waveEnergySize(sourceImage.cols >> scale, sourceImage.rows >> scale);
		  activeWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
		  newWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
		  initActiveWaveEnergy();
	  }

	  NewWaveAnimation(const CvArr *startImg, const CvArr *endImg):
	  Animation(startImg, endImg), sourceImage(cv::cvarrToMat(endImg)), isAlmostWaveEnergyIsZero(false),
		  scale(1), speedUp(false), radius(20), damping(3), initEnergy(1800)
	  {
		  cv::Size waveEnergySize(sourceImage.cols >> scale, sourceImage.rows >> scale);
		  activeWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
		  newWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
		  initActiveWaveEnergy();
	  }

	  void play(Picture &picture);

	  inline void setProperties(const int &damp, 
		  const int &dropRadius = 20, const int &newInitEnergy = 1800) 
	  {		
		  damping = damp;
		  if (radius != dropRadius) {
			  radius = dropRadius;
			  initActiveWaveEnergy();
		  }

		  if (initEnergy != newInitEnergy) {
			  initEnergy = newInitEnergy;
			  initActiveWaveEnergy();
		  }
	  }

	  inline void setSpeedUp(const bool &newSpeedUp) {speedUp = newSpeedUp;}

private:
	void initActiveWaveEnergy();
	void updateWaveEnergy();
	void display(Picture &picture);

private:
	bool isAlmostWaveEnergyIsZero;
	bool speedUp;
	cv::Mat activeWaveEnergy;
	cv::Mat newWaveEnergy;
	cv::Mat sourceImage;
	int scale;
	int radius;
	int damping;
	int initEnergy;
};

#endif // NEWWAVE_ANIMATION_H