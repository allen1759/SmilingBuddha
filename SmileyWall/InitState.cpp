/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>

using namespace std;
using namespace cv;

void InitState::processTime(Director *director, const int64 &currentTickCount)
{

}

void InitState::processKeyEvent(Director *director, const int &key)
{

}

void InitState::processMouseEvent(Director *director, const Point &mousePos)
{

}

void InitState::processOSC(Director *director, OSCListener *oscListener)
{
	if (oscListener->interactionState == INTERACTION_START) {
		director->setAnimationState(new MirrorState);
		director->setStartTickCount();
	}
}

void InitState::processAnimation(Director *director)
{
	director->getOSCListener()->initMessages();

	if (needToPlayDefaultMusic) {
		director->sendOSCBoolMessage("localhost", musicPlayerPort, "/play1", true);
		//director->playMusic("Musics/ScreenFlow.wav");
		needToPlayDefaultMusic = false;
	}

	for (int i = 0; i < numPhotos; ++i) {
		Picture& picture = *director->getPictureAt(i);
		Rect frame = picture.getFrame();
		picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
		picture.setAnimation(IdleAnimationEnum, Mat::zeros(frame.width, 
			frame.height, picture.getType()));
		picture.setEnableAnimationTime(director->getCurrentTime());		
	}
	
	ImagesPool* imagePool = director->getImagesPool();
	imagePool->similarPhotos.clear();

	director->clearCaptureImageVec();
	
	director->setCanRecord(false);
	director->setAnimationState(new PreludeState);
}