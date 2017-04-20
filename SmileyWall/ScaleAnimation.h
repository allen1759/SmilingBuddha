#pragma once
#ifndef SCALE_ANIMATION_H
#define SCALE_ANIMATION_H

#include "Animation.h"
#include "Director.h"
#include <opencv2/opencv.hpp>
#include <vector>

typedef enum VideoState{VIDEO_PLAY_FORWARD, VIDEO_PLAY_BACKWARD, VIDEO_END} VideoState;

class ScaleAnimation : public Animation 
{
public:
	ScaleAnimation(const cv::InputArray &startImg, const cv::InputArray &endImg):
		Animation(startImg, endImg), sourceImage(endImg.getMat()),
			scaleSpeed(0.05), scaleFrom(0), scaleTo(1),
			fadeSpeed(0.1), fadeFrom(0.2), fadeTo(1),
			centerRow(1), centerCol(1), thisRow(2), thisCol(2),
			videoState(VIDEO_PLAY_FORWARD), photoNum(-1), videoCanEnd(false)
		{
			startTime = getTickCount() / getTickFrequency();
			videoStartTime = startTime;
		}

		ScaleAnimation(const CvArr *startImg, const CvArr *endImg):
		Animation(startImg, endImg), sourceImage(cv::cvarrToMat(endImg)),
			scaleSpeed(0.05), scaleFrom(0), scaleTo(1),
			fadeSpeed(0.1), fadeFrom(0.2), fadeTo(1),
			centerRow(1), centerCol(1), thisRow(2), thisCol(2),
			videoState(VIDEO_PLAY_FORWARD), photoNum(-1), videoCanEnd(false)
		{
			startTime = getTickCount() / getTickFrequency();
			videoStartTime = startTime;
		}

		void play(Picture &picture);

		inline void setProperties(std::vector<cv::Mat> photoSet, double runTime,
			int thisRow, int thisCol, int centerRow, int centerCol, 
			double scaleFrom, double scaleTo, double scaleSpeed, double videoStartTime =-1, int photoNum = -1) 
		{
			this->scaleFrom = scaleFrom;
			this->scaleTo = scaleTo;
			this->scaleSpeed = scaleSpeed;
			this->thisRow = thisRow;
			this->thisCol = thisCol;
			this->centerRow = centerRow;
			this->centerCol = centerCol;

			this->photoSet = photoSet;
			this->runTime = runTime;
			if (photoNum == -1) {
				this->photoNum = static_cast<int>(photoSet.size());
			} else {
				this->photoNum = photoNum;
			}
			if (videoStartTime == -1)
				this->videoStartTime = startTime;
			else
				this->videoStartTime = videoStartTime;
		}

private:
	void playVideo(Picture &picture);
	void handleVideoState();
	void scale(double &x, double &y, double dx, double dy);
	void translate(double &x, double &y, double dx, double dy);

	VideoState videoState;
	std::vector<cv::Mat> photoSet;
	int photoNum;
	bool videoCanEnd;

	double startTime, videoStartTime, runTime;
	cv::Mat sourceImage;			 	// overlay image
	int centerRow, centerCol;		// where center of scaled image located in
	int thisRow, thisCol;			// window of picture
	double scaleFrom, scaleTo;
	double scaleSpeed;
	double fadeFrom, fadeTo;
	double fadeSpeed;
};

#endif // SCALE_ANIMATION_H