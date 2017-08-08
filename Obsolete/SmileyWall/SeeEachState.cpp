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
using namespace std;
using namespace cv;

const int select_time = 2;
const double neutral_animation_time = 0.5;
const double neutral2smile_animation_time = 0.5;
const double look_animation_time = 3;
const double smile2smile_time = 0.5;
const int neutral_frame = 7;
const double stay_time = 15;

//const int nineBox[] = {12, 13, 14, 21, 22, 23, 30, 31, 32};

void SeeEachState::processTime(Director *director, const int64 &currentTickCount)
{
	currentTime = currentTickCount/getTickFrequency();
	if (lastSelectTime == -1) {
		lastSelectTime = static_cast<int>(currentTime);
	}
	if (startTime == -1) {
		startTime = currentTime;
	}
}

void SeeEachState::processKeyEvent(Director *director, const int &key)
{
	switch(key) {
	case 't': case 'T':
		director->takePhoto(photo);
		break;
	case RIGHT_ARROW:
		director->setCanRecord(false);
		director->setAnimationState(new EndingState, true);
		break;
	case 's':
		userSmile = true;
		break;
	default:
		break;
	}
}

void SeeEachState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

void SeeEachState::processOSC(Director *director, OSCListener *oscListener)
{
#if SIMULATOR == 0
	Point2f eyePos(0, 0);
	eyePos = director->getEyePosWithOSCMessage();
	if (eyePos != Point2f(0, 0)){
		//index = director->get2DEyePos(eyePos);
		eyePosIndex = director->get1DEyePos(eyePos);
	}

	//if (!readedUserPhoto && seeEachStep == DETECT_SMILE) {
		if(OSCFlag == false && director->takeVideoFromSmileDetect()) {
			userSmile = true;
			
		}
	//}
#endif
}

void SeeEachState::processAnimation(Director *director)
{
	if (userNo != director->getCurrentVideoNum()-1) {
		readedUserPhoto = true;
	}
	ImagesPool *imagesPool = director->getImagesPool();
	switch (seeEachStep) {
	case SEEEACH_INITIAL:
		for(int i=0;i<numPhotos;i++){
			Picture& picture2 = *director->getPictureAt(i);
			picture2.setContent(Mat(picture2.size(), picture2.getType(), Scalar(0, 0, 0)));
		}
		for(int i=0;i<9;i++){
			Picture& picture2 = *director->getPictureAt(nineBox[i]);
			picture2.setContent(director->getImagesPool()->allLookPhotos.at(nineBox[i]%numLookPeople).at(5).at(0));
		}
		firstStayTime = currentTime;
		seeEachStep = DETECT_SMILE;
		director->getImagesPool()->currentUserPhotos.clear();
		director->clearOscVideoInfo();
		break;
	case DETECT_SMILE:

		if (OSCFlag && currentTime - firstStayTime > 3) {
			director->sendOSCBoolMessage("localhost", smileDatectionPort, "/interaction", true);
			OSCFlag = false;
		}

		if(userSmile && director->isVideoReadFin()) { //smile
			director->setCurrentVideoNum(1);
			seeEachStep = AFTER_SMILE;
		}

		for (int i=0; i<9; i++) {
			Picture& picture = *director->getPictureAt(nineBox[i]);
			if (nineBox[i] == selectPair[0] || nineBox[i] == selectPair[1]) {
				continue;
			}
			if (!picture.getAnimation() || picture.getAnimation()->animationEnded()) {
				picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(0).at(0), 
					imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setCanReverse(true);
				lookAnimation->setProperties(imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(0), neutral_animation_time, neutral_frame);
				picture.setEnableAnimationTime(director->getCurrentTime());
				pictureState[nineBox[i]] = 0;
			}
		}

		switch (selectState) {
		case 0:
			selectPairForSeeEach();
			selectState = 1;
			break;
		case 1:
			if (isSelectAniEnd(director)) {
				//add neutral2smile animation;
				for (int i=0; i<2; i++) {
					Picture& picture = *director->getPictureAt(selectPair[i]);
					picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(0).at(0), 
						imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(0).at(0));

					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					lookAnimation->setCanReverse(false);
					lookAnimation->setProperties(imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(0), neutral2smile_animation_time);
					picture.setEnableAnimationTime(director->getCurrentTime());
					pictureState[i] = 0;
				}
				selectState = 2;
			}
			break;
		case 2:
			if (isSelectAniEnd(director)) {
				for (int i=0; i<2; i++)
				{
					//add smile smooth animation;
					int direction = whereOtherSide(selectPair[1-i], selectPair[i]);
					Picture& picture = *director->getPictureAt(selectPair[i]);
					picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(0).at(0), 
						imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(0).at(0));

					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					lookAnimation->setCanReverse(false);
					lookAnimation->setProperties(imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(9+direction), neutral2smile_animation_time);
					picture.setEnableAnimationTime(director->getCurrentTime());
					pictureState[i] = direction;
				}
				selectState = 3;
			}
			break;
		case 3:
			if (isSelectAniEnd(director)) {
				for (int i=0; i<2; i++)
				{
					Picture& picture = *director->getPictureAt(selectPair[i]);
					picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(5).at(0), 
						imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(5).at(0));

					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					lookAnimation->setCanReverse(true);
					int direction = whereOtherSide(selectPair[1-i], selectPair[i]);
					//printf("%d,%d %d\n",selectPair[0], selectPair[1], direction);
					assert (direction != 5);
					lookAnimation->setProperties(imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(direction), look_animation_time);
					picture.setEnableAnimationTime(director->getCurrentTime());
				}
				selectState = 4;
			}
			break;
		case 4:
			if (isSelectAniEnd(director)) {
				for (int i=0; i<2; i++)
				{
					//add reverse smile smooth animation;
					int direction = whereOtherSide(selectPair[1-i], selectPair[i]);
					Picture& picture = *director->getPictureAt(selectPair[i]);
					picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), 
						imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));

					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					lookAnimation->setCanReverse(false);
					lookAnimation->setProperties(imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(9+direction), neutral2smile_animation_time, -1, LOOK_2);
					picture.setEnableAnimationTime(director->getCurrentTime());
					pictureState[i] = 0;
				}
				selectState = 5;
			}
			break;
		case 5:
			if (isSelectAniEnd(director)) {
				for (int i=0; i<2; i++)
				{
					//add smile2neutral animation;
					int direction = whereOtherSide(selectPair[1-i], selectPair[i]);
					Picture& picture = *director->getPictureAt(selectPair[i]);
					picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), 
						imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));

					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					lookAnimation->setCanReverse(false);
					lookAnimation->setProperties(imagesPool->allLookPhotos.at(selectPair[i]%numLookPeople).at(0), neutral2smile_animation_time, -1, LOOK_2);
					picture.setEnableAnimationTime(director->getCurrentTime());
					pictureState[i] = -1;
				}
				selectState = 6;
			}
			break;
		case 6:
			if (isSelectAniEnd(director)) {
				selectState = 0;
			}
			break;
		}
		break;
	case PLAYANI:
		
		aniFinishFlag = true;
		for(int i=0; i<9; i++){
			if (animationType[nineBox[i]] != CENTERANI) {
				aniFinishFlag = false;
				break;
			}
		}

		if (aniFinishFlag) {
			bool switch2interaction = true;
			for(int i=0; i<9; i++){
				Picture& picture = *director->getPictureAt(nineBox[i]);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					switch2interaction = false;
					break;
				}
			}
			if (switch2interaction) {
				director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
				director->setAnimationState(new GazeState, true);
			}
		}

		for(int i=0; i<9; i++){
			Picture& picture = *director->getPictureAt(nineBox[i]);
			if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
				continue;
			}

			if (aniFinishFlag) {
				picture.setContent(imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(0).at(0));
				continue;
			}

			if (animationType[nineBox[i]] == CENTERANI) {
				//SetCENTERAnimation(picture, imagesPool, i, director);
				picture.setContent(imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(0).at(0));
				continue;
			}

			if (i == 4)
			{
				switch (animationType[22]) {
					case ANISTATRT:
						{
							director->sendOSCBoolMessage("localhost", musicPlayerPort, "/play3", true);
							picture.setAnimation(LookAnimationEnum, picture);

							LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
							lookAnimation->setProperties(imagesPool->currentUserPhotos.at(0), videoTime);
							lookAnimation->setCanReverse(true);

							picture.setEnableAnimationTime(director->getCurrentTime());	
							picture.addAnimation(NewWaveAnimationEnum, imagesPool->currentUserPhotos.at(0).at(0), imagesPool->currentUserPhotos.at(0).at(0));
							animationType[22] = PHOTOVIDEO;
						}
						break;
					case PHOTOVIDEO:
						{
							director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
			
							picture.setAnimation(WaveAnimationEnum, imagesPool->currentUserPhotos.at(0).at(numUserPhotos-1), imagesPool->allLookPhotos.at(22%numLookPeople).at(0).at(0));
						

							WaveAnimation *waveAnimation = static_cast<WaveAnimation *>(picture.getAnimation());
							picture.setEnableAnimationTime(director->getCurrentTime());
							animationType[22] = PHOTOwave2SMILE;
						}
						break;
					case PHOTOwave2SMILE:
						{
							//SetCENTERAnimation(picture, imagesPool, i, director);
							picture.setContent(imagesPool->allLookPhotos.at(22%numLookPeople).at(0).at(0));
							animationType[22] = CENTERANI;
						}
						break;
				}
			} else {
				Vec2d vec((nineBox[i] % numPicturesInPainting.width) - (22 % numPicturesInPainting.width),
					(nineBox[i] / numPicturesInPainting.width) - (22 / numPicturesInPainting.width));
				switch (animationType[nineBox[i]]) {
				case ANISTATRT:
					{
						if (pictureState[nineBox[i]] == PHOTO2SMILE) {
							//cout << currentTime - waveStartTime << "\n";
						}
						int positionDirector = -1;
						if (vec[0] < 0 && vec[1] < 0) {
							positionDirector = 1;
						} else if (vec[0] == 0 && vec[1] < 0) {
							positionDirector = 2;
						} else if (vec[0] > 0 && vec[1] < 0) {
							positionDirector = 3;
						} else if (vec[0] < 0 && vec[1] == 0) {
							positionDirector = 4;
						} else if (vec[0] > 0 && vec[1] == 0) {
							positionDirector = 6;
						} else if (vec[0] < 0 && vec[1] > 0) {
							positionDirector = 7;
						} else if (vec[0] == 0 && vec[1] > 0) {
							positionDirector = 8;
						} else if (vec[0] > 0 && vec[1] > 0) {
							positionDirector = 9;
						} else if (vec[0] == 0 && vec[1] == 0) {
							positionDirector = 0;
							printf("position center: %d\n", 22);
							assert(false);
						}
						assert(positionDirector > -1);
						//add morphong smile
						SetDirectionMorphAnimation( picture, imagesPool, nineBox[i],  0, i+1, director );
						animationType[nineBox[i]] = CENTER2DIR;
						//pictureDirection[nineBox[i]] = positionDirector;
					}
					break;
				case CENTER2DIR:
					{
						picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(i+1).at(0), 
							imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(i+1).at(0));

						LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
						lookAnimation->setCanReverse(true);
						lookAnimation->setProperties(imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(i+1), animation_time);

						int64 timeOffset = static_cast<int64>(getTickFrequency() * max(abs(vec[0]),abs(vec[1])) * animation_timeoffset);
						picture.setEnableAnimationTime(director->getCurrentTime() + timeOffset + static_cast<int64>(rand()%10 * 0 * getTickFrequency()));

						animationType[nineBox[i]] = DIRANI;
					}
					break;
				case DIRANI:
					{
						int positionDirector = 0;
						SetDirectionMorphAnimation( picture, imagesPool, nineBox[i], i+1, 0, director );
						animationType[nineBox[i]] = MORPH2CENTER;
					}
					break;
				case MORPH2CENTER:
					{
						//SetCENTERAnimation(picture, imagesPool, i, director);
						picture.setContent(imagesPool->allLookPhotos.at(nineBox[i]%numLookPeople).at(0).at(0));
						animationType[nineBox[i]] = CENTERANI;
					}
					break;
				}
			}
		}
		break;
	case AFTER_SMILE:
		bool allAnimationEnd = true;
		for (int i=0; i<9; i++) {
			Picture& picture = *director->getPictureAt(nineBox[i]);
			picture.getAnimation()->setEnd(true);
		}
		if (allAnimationEnd) {
			seeEachStep = PLAYANI;
			//director->setAnimationState(new BroadcastState, true);
		}
		break;
	}
}

int SeeEachState::whereOtherSide( int aIndex, int bIndex ) 
{
	Vec2d vec((bIndex % numPicturesInPainting.width) - (aIndex % numPicturesInPainting.width),
		(bIndex / numPicturesInPainting.width) - (aIndex / numPicturesInPainting.width));
	int positionDirector = -1;
	if (vec[0] < 0 && vec[1] < 0)
	{
		positionDirector = 1;
	} else if (vec[0] == 0 && vec[1] < 0)
	{
		positionDirector = 2;
	} else if (vec[0] > 0 && vec[1] < 0)
	{
		positionDirector = 3;
	} else if (vec[0] < 0 && vec[1] == 0)
	{
		positionDirector = 4;
	} else if (vec[0] > 0 && vec[1] == 0)
	{
		positionDirector = 6;
	} else if (vec[0] < 0 && vec[1] > 0)
	{
		positionDirector = 7;
	} else if (vec[0] == 0 && vec[1] > 0)
	{
		positionDirector = 8;
	} else if (vec[0] > 0 && vec[1] > 0)
	{
		positionDirector = 9;
	} else if (vec[0] == 0 && vec[1] == 0)
	{
		positionDirector = 5;
	}
	assert(positionDirector > -1);
	return positionDirector;
}

void SeeEachState::selectPairForSeeEach() 
{
	int pairnum = rand()%20; //0-5->'-', 6-11->'|', 12-15->'\', 16-19->'/'(L->R, U->D)
	switch(pairnum) {
	case 0:
		selectPair[0] = 12;
		selectPair[1] = 13;
		break;
	case 1:
		selectPair[0] = 13;
		selectPair[1] = 14;
		break;
	case 2:
		selectPair[0] = 21;
		selectPair[1] = 22;
		break;
	case 3:
		selectPair[0] = 22;
		selectPair[1] = 23;
		break;
	case 4:
		selectPair[0] = 30;
		selectPair[1] = 31;
		break;
	case 5:
		selectPair[0] = 31;
		selectPair[1] = 32;
		break;
	case 6:
		selectPair[0] = 12;
		selectPair[1] = 21;
		break;
	case 7:
		selectPair[0] = 13;
		selectPair[1] = 22;
		break;
	case 8:
		selectPair[0] = 14;
		selectPair[1] = 23;
		break;
	case 9:
		selectPair[0] = 21;
		selectPair[1] = 30;
		break;
	case 10:
		selectPair[0] = 22;
		selectPair[1] = 31;
		break;
	case 11:
		selectPair[0] = 23;
		selectPair[1] = 32;
		break;
	case 12:
		selectPair[0] = 12;
		selectPair[1] = 22;
		break;
	case 13:
		selectPair[0] = 13;
		selectPair[1] = 23;
		break;
	case 14:
		selectPair[0] = 21;
		selectPair[1] = 31;
		break;
	case 15:
		selectPair[0] = 22;
		selectPair[1] = 32;
		break;
	case 16:
		selectPair[0] = 13;
		selectPair[1] = 21;
		break;
	case 17:
		selectPair[0] = 14;
		selectPair[1] = 22;
		break;
	case 18:
		selectPair[0] = 22;
		selectPair[1] = 30;
		break;
	case 19:
		selectPair[0] = 23;
		selectPair[1] = 31;
		break;
	default:
		printf("pairnum error: %d\n", pairnum);
		assert(false);
	}
}

bool SeeEachState::isSelectAniEnd( Director * director ) 
{
	bool selectAniEnd = true;
	for (int i=0; i<2; i++)
	{
		Picture& picture = *director->getPictureAt(selectPair[i]);
		if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
			selectAniEnd = false;
		}
	}
	return selectAniEnd;
}

void SeeEachState:: SetDirectionMorphAnimation
	( Picture &picture, ImagesPool * imagesPool, int i, int first, int second, Director * director ) 
{
	picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), 
		imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
	//if (pictureDirection[i] != positionDirector) {
		LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
		lookAnimation->setCanReverse(false);
		int smoothPair[2] = {0, 0};
		bool reverse = false;
		if (first < second) {
			smoothPair[0] = first;
			smoothPair[1] = second;
		} else {
			smoothPair[1] = first;
			smoothPair[0] = second;
			reverse = true;
		}
		int smoothIndex = 9;
		for (int si=0; si<smoothPair[0]; si++) {
			smoothIndex += (9-si);
		}
		smoothIndex += smoothPair[1] - smoothPair[0];
		if (!reverse) {
			lookAnimation->setProperties(imagesPool->allLookPhotos.at(i%numLookPeople).at(smoothIndex), smooth_time);
		} else {
			lookAnimation->setProperties(imagesPool->allLookPhotos.at(i%numLookPeople).at(smoothIndex), smooth_time, -1, LOOK_2);
		}
		picture.setEnableAnimationTime(director->getCurrentTime());
	//}
}