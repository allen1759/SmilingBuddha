#pragma once
#ifndef LOOKWAVE_ANIMATION_H
#define LOOKWAVE_ANIMATION_H

#include "Animation.h"
#include "Director.h"
#include <opencv2/opencv.hpp>
#include <vector>

typedef enum LookWaveState{LOOKWAVE_PLAY_FORWARD, LOOKWAVE_PLAY_BACKWARD, LOOKWAVE_END} LookWaveState;

class LookWaveAnimation : public Animation 
{
public:
	LookWaveAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg):
		Animation(startImg, endImg), sourceImage(endImg.getMat()),
			lookWaveState(LOOKWAVE_PLAY_FORWARD), photoNum(-1), videoCanEnd(false),
			isAlmostWaveEnergyIsZero(false), scale(1), speedUp(false), radius(20), damping(3), initEnergy(1800)
		{
			videoStartTime = getTickCount() / getTickFrequency();
			cv::Size waveEnergySize(sourceImage.cols >> scale, sourceImage.rows >> scale);
			activeWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
			newWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
			initActiveWaveEnergy();
		}

	LookWaveAnimation(const CvArr *startImg, const CvArr *endImg):
		Animation(startImg, endImg), sourceImage(cv::cvarrToMat(endImg)),
			lookWaveState(LOOKWAVE_PLAY_FORWARD), photoNum(-1), videoCanEnd(false),
			isAlmostWaveEnergyIsZero(false), scale(1), speedUp(false), radius(20), damping(3), initEnergy(1800)
		{
			videoStartTime = getTickCount() / getTickFrequency();
			cv::Size waveEnergySize(sourceImage.cols >> scale, sourceImage.rows >> scale);
			activeWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
			newWaveEnergy = cv::Mat::zeros(waveEnergySize, CV_16SC1);
			initActiveWaveEnergy();
		}

		void play(Picture &picture);

		inline void setProperties(std::vector<cv::Mat> photoSet, double runTime, const int &damp = 3, 
			const int &dropRadius = 20, const int &newInitEnergy = 1800, int photoNum = -1)
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

			this->photoSet = photoSet;
			this->runTime = runTime;

			if (photoNum == -1) {
				this->photoNum = static_cast<int>(photoSet.size());
			} else {
				this->photoNum = photoNum;
			}
		}

		inline void setSpeedUp(const bool &newSpeedUp) {speedUp = newSpeedUp;}

private:
	void initActiveWaveEnergy();
	void updateWaveEnergy();
	void display(Picture &picture);
	void playVideo(Picture &picture);
	void handleVideoState();

	LookWaveState lookWaveState;
	std::vector<cv::Mat> photoSet;
	int photoNum;
	bool videoCanEnd;
	double videoStartTime, runTime;

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

#endif // LOOKWAVE_ANIMATION_H