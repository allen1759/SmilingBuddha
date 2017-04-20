/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace cv;

//const vector<int> nineBox = {12, 13, 14, 21, 22, 23, 30, 31, 32};

void GazeState::processTime(Director *director, const int64 &currentTickCount)
{
	currentTime = currentTickCount/getTickFrequency();
}

void GazeState::processKeyEvent(Director *director, const int &key)
{
	switch(key) {
	case RIGHT_ARROW:
		director->setCanRecord(false);
		director->setAnimationState(new EndingState, true);
		break;
	default:
		break;
	}
}

void GazeState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

void GazeState::processOSC(Director *director, OSCListener *oscListener)
{
#if SIMULATOR == 0
	Point2f eyePos(0, 0);
	eyePos = director->getEyePosWithOSCMessage();
	if (eyePos != Point2f(0, 0)){
		//index = director->get2DEyePos(eyePos);
		eyePosIndex = director->get1DEyePos(eyePos);
	}
#endif
}

void GazeState::processAnimation(Director *director)
{
	if (waitOSC && currentTime - waitOSCstartTime > videoTime) {
		director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
		waitOSC = false;
	}
	
	switch (gazeStep) {
	case GAZE_INITIAL:
		stateStartTime = currentTime;
		for (int i=0; i<numPhotos; i++)
		{
			if (find(nineBox, nineBox+9, i) != nineBox+9 || find(firstLineIndex, firstLineIndex+8, i) != firstLineIndex+8) {
				cout << i << "\n";
				isDisplay[i] = true;
			} else {
				isDisplay[i] = false;
			}
		}
		gazeStep = FIRSTLINE;
		return;
	case FIRSTLINE:
		if (currentTime - stateStartTime > 2)
		{
			for (int i=0; i<numPhotos; i++)
			{
				if (find(secondLineIndex, secondLineIndex+8, i) != secondLineIndex+8) {
					isDisplay[i] = true;
				}
			}
			
			gazeStep = SECONDLINE;
		}

		break;
	case SECONDLINE:
		if (currentTime - stateStartTime > 3)
		{
			/*for (int i=0; i< numPhotos; i++)
			{
				Picture& picture = *director->getPictureAt(i);
				if(picture.getAnimation()) {
					picture.getAnimation()->setEnd(true);
				}
			}*/
			director->setAnimationState(new BroadcastState(), false);
			return;
		}
		break;
	}

	/*if (waitOSC && currentTime - waitOSCstartTime > videoTime) {
		director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
		waitOSC = false;
	}*/

	if (userNo != director->getCurrentVideoNum()-1) {
		userNo = director->getCurrentVideoNum()-1;
		waitOSC = true;
		waitOSCstartTime = currentTime;	
	}
	
	ImagesPool *imagesPool = director->getImagesPool();
	{
		Picture& lastAddPicture = *director->getPictureAt(currentEyePosIndex);

		if (addFlag) {
			Animation * lastAddAnimation = lastAddPicture.getLastAnimation();
			if (lastAddAnimation) {
				lastAddAnimation->setEnd(true);
				lastAddPicture.popAnimation();
			}
			addFlag = false;
		}
			
			
			
		int isCurrentNneighborOfLast = 0;
		int pos = 0;
		bool playWave = false;

		if(first == 1){
			first = 2;
			starttime = currentTime;
			attpt = eyePosIndex;
		}
		else if(first == 2){
			//if(2 <= attpt && attpt<= 8){
			if(1 <= attpt && attpt<= 7){
				pos = 2;
				if((eyePosIndex == attpt) ||(eyePosIndex == attpt - 1) || (eyePosIndex == attpt + 1) || (eyePosIndex == attpt + 8) || (eyePosIndex == attpt + 9) || (eyePosIndex == attpt + 10)){
					isCurrentNneighborOfLast = 1;
				}
			}
			//else if(35 <= attpt && attpt<= 41){
			else if(28 <= attpt && attpt<= 34){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == attpt - 1) || (eyePosIndex == attpt + 1) || (eyePosIndex == attpt - 10) || (eyePosIndex == attpt - 9) || (eyePosIndex == attpt - 8)){
					isCurrentNneighborOfLast = 1;
				}

			}
			//else if(attpt == 12 || attpt == 23){
			else if(attpt == 9 || attpt == 18){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == attpt - 9) || (eyePosIndex == attpt - 8) || (eyePosIndex == attpt + 1) || (eyePosIndex == attpt + 9) || (eyePosIndex == attpt + 10)){
					isCurrentNneighborOfLast = 1;
				}
			}
			//else if(attpt == 20 || attpt == 31){
			else if(attpt == 17 || attpt == 26){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == attpt - 10) || (eyePosIndex == attpt - 9) || (eyePosIndex == attpt - 1) || (eyePosIndex == attpt + 8) || (eyePosIndex == attpt + 9)){
					isCurrentNneighborOfLast = 1;
				}
			}
			//else if(attpt == 1){
			else if(attpt == 0){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == 1) || (eyePosIndex == 9) || (eyePosIndex == 10)){
					isCurrentNneighborOfLast = 1;
				}
			}
			//else if(attpt == 9){
			else if(attpt == 8){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == 7) || (eyePosIndex == 16) || (eyePosIndex == 17)){
					isCurrentNneighborOfLast = 1;
				}
			}
			//else if(attpt == 34){
			else if(attpt == 27){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == 18) || (eyePosIndex == 19) || (eyePosIndex == 28)){
					isCurrentNneighborOfLast = 1;
				}
			}
			//else if(attpt == 42){
			else if(attpt == 35){
				pos = 2;
				if((eyePosIndex == attpt) || (eyePosIndex == 25) || (eyePosIndex == 26) || (eyePosIndex == 34)){
					isCurrentNneighborOfLast = 1;
				}
			}
			else{
				pos = 1;
				if((eyePosIndex == attpt) || (eyePosIndex == attpt - 1) || (eyePosIndex == attpt + 1) || (eyePosIndex == attpt -10) || (eyePosIndex == attpt - 9) || (eyePosIndex == attpt - 8) || (eyePosIndex == attpt + 8) || (eyePosIndex == attpt + 9) || (eyePosIndex == attpt + 10)){
					isCurrentNneighborOfLast = 1;
				}
			}
			if(isEyeStayMoreThenThreshold ==0){
				if(eyePosIndex == attpt){
					if(pos == 1){
						if((currentTime - starttime) >= msec){  //after "sec", get new position
							isEyeStayMoreThenThreshold = 1;
						}
					}
					else if(pos ==2){
						if((currentTime - starttime) >= bsec){  //after "sec", get new position
							isEyeStayMoreThenThreshold = 1;
						}
					}
				}
				else{
					first = 1;
				}
			}
			else if(isEyeStayMoreThenThreshold == 1){
				if(isCurrentNneighborOfLast != 1){
					first = 1;
				}
				if((currentTime - starttime) >= eye_stay_time){  //after "sec", get new position
					playWave = true;
				}
			}
		}


		if(isEyeStayMoreThenThreshold == 0 ||( isEyeStayMoreThenThreshold == 1 && isCurrentNneighborOfLast == 0)){     // not locked and break the lock
			currentEyePosIndex = eyePosIndex;
		}
		else{   //lock, show attempted position
			currentEyePosIndex = attpt;
		}

		if (!isDisplay[currentEyePosIndex])
		{
			isDisplay[currentEyePosIndex] = true;
		}

		for(int i=0; i<numPhotos; i++){
			Picture& picture = *director->getPictureAt(i);
			if (!isDisplay[i]) {
				continue;
			}
			if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
				continue;
			}
			SetCENTERAnimation(picture, imagesPool, i, director);
		}

		Picture& picture = *director->getPictureAt(currentEyePosIndex);
		picture.addAnimation(ChangeBackgroundColorAnimationEnum, picture, picture);
		ChangeBackgroundColorAnimation *cbcAnimation = static_cast<ChangeBackgroundColorAnimation *>(picture.getLastAnimation());
		if (currentTime - starttime > 0) {
			cbcAnimation->setProperty((int)(((currentTime - starttime) / eye_stay_time)*cbc_frame));
		} else {
			cbcAnimation->setProperty(0);
		}
		addFlag = true;

		/*if (playWave) {
			Picture& picture = *director->getPictureAt(currentEyePosIndex);
			picture.getAnimation()->setEnd(true);
			Animation *lastAddAnimation = lastAddPicture.getLastAnimation();
			lastAddAnimation->setEnd(true);
			lastAddPicture.popAnimation();
			addFlag = false;
		}*/
	}
}

void GazeState::SetCENTERAnimation( Picture &picture, ImagesPool * imagesPool, int i, Director * director ) 
{
	picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), 
		imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));

	LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
	lookAnimation->setCanReverse(true);
	lookAnimation->setProperties(imagesPool->allLookPhotos.at(i%numLookPeople).at(0));
	picture.setEnableAnimationTime(director->getCurrentTime());
}