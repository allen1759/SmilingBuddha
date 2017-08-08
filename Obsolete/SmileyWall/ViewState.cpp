/*!
 *\author YuShan Lin
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include <iostream>

using namespace std;
using namespace cv;

void ViewState::processTime(Director *director, const int64 &currentTickCount)
{

}

void ViewState::processKeyEvent(Director *director, const int &key)
{
//#if SIMULATOR == 1
	if (key == 'w') {
		currentUserNum ++;
		if (currentUserNum >= director->getImagesPool()->allLookPhotos.size()) {
			currentUserNum = 0;
		}
		for (int i=0; i<9; i++) {
			Picture& picture = *director->getPictureAt(m[i]);
			picture.getAnimation()->setEnd(true);
		}
	}
	else if (key == 's') {
		currentUserNum --;
		if (currentUserNum < 0) {
			currentUserNum = director->getImagesPool()->allLookPhotos.size()-1;
		}
		for (int i=0; i<9; i++) {
			Picture& picture = *director->getPictureAt(m[i]);
			picture.getAnimation()->setEnd(true);
		}
	}
//#endif
}

void ViewState::processMouseEvent(Director *director, const Point &mousePos)
{
}

void ViewState::processOSC(Director *director, OSCListener *oscListener)
{

}

void ViewState::processAnimation(Director *director)
{	
	if (firstcome) {
		for (int i = 0; i < 44; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.getAnimation()->setEnd(true);
			picture.setContent(director->getImagesPool()->textPhotos.at(5));
		}
		firstcome = false;
	}
	

	for (int i = 0; i < 9; i++) {
		Picture& picture = *director->getPictureAt(m[i]);
		if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
			continue;
		}
		picture.setAnimation(LookAnimationEnum, picture);

		LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
		lookAnimation->setCanReverse(true);

			lookAnimation->setProperties(director->getImagesPool()->allLookPhotos.at(currentUserNum).at(10+i), 2.0f);
		
		picture.setEnableAnimationTime(director->getCurrentTime());
	}
	
}