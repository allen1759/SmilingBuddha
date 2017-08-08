/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "LookAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#include "Director.h"

using namespace cv;
using namespace std;

void LookAnimation::play(Picture &picture)
{
	if (startTime == -1)
	{
		startTime = getTickCount() / getTickFrequency();
	}
	currentTime = getTickCount() / getTickFrequency();
	handleLookState();
	display(picture);
}

void LookAnimation::initWhatNeed()
{
	startTime = -1;
	firstPhotoNum = 0;
}

void LookAnimation::handleLookState()
{
	switch(lookState) {
	case LOOK_1:
		if (currentTime-startTime >= runtime) {
			if (canReverse) {
				lookState = LOOK_2;
				startTime = currentTime;
				runtime = runtime;
				firstPhotoNum = 0;
				canReverse = false;
			} else {
				lookState = LOOK_END;
				ended = true;
			}
		}
		break;
	case LOOK_2:
		if (currentTime-startTime >= runtime) {
			lookState = LOOK_END;
			ended = true;
		}
		break;
	case LOOK_END:
		ended = true;
		break;
	default:
		break;
	}
}

void LookAnimation::display(Picture &picture)
{
	int currentPhotoIndex = firstPhotoNum + (int)((currentTime - startTime)/runtime*photoNum);
	if (currentPhotoIndex < 0 || currentPhotoIndex >= static_cast<int>(photoSet.size()))
	{
		return;
	}
	switch(lookState) {
	case LOOK_1:
		picture.setContent(photoSet.at(currentPhotoIndex));
		break;
	case LOOK_2:
		currentPhotoIndex = photoNum - 1 - currentPhotoIndex;
		picture.setContent(photoSet.at(currentPhotoIndex));
		break;
	case LOOK_END:
		break;
	default:
		break;
	}
}
