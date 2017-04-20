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

const double play_time = 35;
//const double eye_stay_time = 3;
//const double animation_timeoffset = 0.1;
//const double animation_time = 5.5;
const double wave_interval = 0.3;
const double delta = 0;
//const double smooth_time = 0.5;
const double takephoto_time = 3;
const int center_ani_frame = 10;
const double center_ani_time = 1;
const double end_video_time = 30;
const double end_black_time = 45;

//const float bsec = 0.7;
//const float msec = 1.12;
//const int cbc_frame = 28;

const int end_video_table[] = {18, 3, 29, 6, 35, 12, 22, 9, 25,
							33, 23, 11, 26, 15, 19, 2, 32, 5,
							8, 30, 20, 34, 1, 24, 28, 16, 36,
							27, 14, 4, 17, 31, 7, 13, 21, 10};


void BroadcastState::processTime(Director *director, const int64 &currentTickCount)
{
	currentTime = currentTickCount/getTickFrequency();
	if (startTime == -1) {
		startTime = currentTime;
		//director->sendOSCBoolMessage("localhost", musicPlayerPort, "/play3", true);
		//director->sendOSCBoolMessage("localhost", musicPlayerPort, "/event2", true);
		/*director->playMusic("Musics/loopB.wav");
		director->playMusic(bellPath,false, false);*/
		/*director->playMusic(musicPath[musicCounter%4],false, false);
		musicCounter++;*/
	}
}

void BroadcastState::processKeyEvent(Director *director, const int &key)
{
	switch(key) {
	case 't': case 'T':
		director->takePhoto(photo);
		break;
	case 'f': case 'F':
		blackFlag = true;
		break;
	case RIGHT_ARROW:
		director->setCanRecord(false);
		director->setAnimationState(new EndingState, true);
		break;
	default:
		break;
	}
}

void BroadcastState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
	for(int i=4; i>0; i--) {
		eyePosIndexArr[i] = eyePosIndexArr[i-1];
	}
	eyePosIndexArr[0] = eyePosIndex;
#endif
}

void BroadcastState::processOSC(Director *director, OSCListener *oscListener)
{
#if SIMULATOR == 0
	eyePos = director->getEyePosWithOSCMessage();
	if (eyePos != Point2f(0, 0)){
		//index = director->get2DEyePos(eyePos);
		eyePosIndex = director->get1DEyePos(eyePos);
	}

	lastSmileState = currentSmileState;
	currentSmileState = director->getSmileStateWithOSCMessage();
	
	director->takeVideoFromSmileDetect();

	for(int i=4; i>0; i--) {
		eyePosIndexArr[i] = eyePosIndexArr[i-1];
	}
	eyePosIndexArr[0] = eyePosIndex;
#endif
}

void BroadcastState::processAnimation(Director *director)
{
	
	ImagesPool *imagesPool = director->getImagesPool();

	/*if (playSmileAnime && broadcastStep != ENDVIDEO && broadcastStep != ENDBUDDHA) {
		Picture& picture = *director->getPictureAt(22);
		if (picture.getAnimation()->animationEnded()) {
			playSmileAnime = false;
			picture.setContent(imagesPool->allLookPhotos.at(22%numLookPeople).at(0).at(0));
			director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
		}
	}*/
	if (waitOSC && currentTime - waitOSCstartTime > videoTime) {
		director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
		waitOSC = false;
	}

	switch(broadcastStep) {
	case BROADCAST_INITIAL:
		lastTakePhotoTime = currentTime;
		lastEyePosIndex = eyePosIndex;
		firstStayTime = currentTime;
		userNo = director->getCurrentVideoNum()-1;
		first = 1;
		isEyeStayMoreThenThreshold = 0;
		starttime = 0;
		broadcastStep = INTERACT;
		currentCenter = 22;
		break;
	case ANIMATION:
		if (userNo != director->getCurrentVideoNum()-1) {
			//director->sendOSCBoolMessage("localhost", musicPlayerPort, "/event2", true);
			/*director->playMusic(musicPath[musicCounter%4],false, false);
			musicCounter++;*/
			userNo = director->getCurrentVideoNum()-1;
			waitOSC = true;
			waitOSCstartTime = currentTime;
			/*playSmileAnime = true;
			Picture& picture = *director->getPictureAt(22);
			picture.getAnimation()->setEnd(true);
			picture.setAnimation(LookAnimationEnum, picture);

			LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
			lookAnimation->setProperties(director->getImagesPool()->currentUserPhotos.at(userNo), videoTime);

			picture.setEnableAnimationTime(director->getCurrentTime());*/
		}

		aniFinishFlag = true;
		for(int i=0; i<numPhotos; i++){
			if (animationType[i] != CENTERANI) {// && !(playSmileAnime && i==22)) {
				aniFinishFlag = false;
				break;
			}
		}

		if (aniFinishFlag) {
			bool switch2interaction = true;
			for(int i=0; i<numPhotos; i++){
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()){// && !(playSmileAnime && i==22)) {
					switch2interaction = false;
					break;
				}
			}
			if (switch2interaction) {
				if (currentTime - startTime > play_time) {
					for(int i=0; i<numPhotos; i++){
						Picture& picture = *director->getPictureAt(i);
						picture.getAnimation()->setEnd(true);
						animationType[i] = ANISTATRT;
					}
					startEndVideoTime = currentTime;
					director->sendOSCBoolMessage("localhost", smileDatectionPort, "/interaction", false);
					//director->sendTopUserVideo();
					broadcastStep = ENDVIDEO;
				} else {
					firstStayTime = currentTime;
					first = 1;
					isEyeStayMoreThenThreshold = 0;
					starttime = 0;
					broadcastStep = INTERACT;
				}
			}
		}

		for(int i=0; i<numPhotos; i++){
			Picture& picture = *director->getPictureAt(i);
			if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
				continue;
			}

			if (aniFinishFlag) {
				picture.setContent(imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
				continue;
			}

			if (animationType[i] == CENTERANI) {
				//SetCENTERAnimation(picture, imagesPool, i, director);
				picture.setContent(imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
				continue;
			}

			if (i == currentCenter)
			{
				switch (animationType[i]) {
					case ANISTATRT:
						{
							picture.setAnimation(LookAnimationEnum, picture);

							LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
							lookAnimation->setProperties(imagesPool->currentUserPhotos.at(currentAniIndex), videoTime);
							lookAnimation->setCanReverse(true);

							picture.setEnableAnimationTime(director->getCurrentTime());	
							picture.addAnimation(NewWaveAnimationEnum, imagesPool->currentUserPhotos.at(currentAniIndex).at(0), imagesPool->currentUserPhotos.at(currentAniIndex).at(0));
							animationType[i] = PHOTOVIDEO;
						}
						currentAniIndex = userNo;
						lookIndexVector.push_back(currentCenter);
						lookVideoVector.push_back(userNo);
						break;
					case PHOTOVIDEO:
						{
							/*if (firstPlayVideo) {
								director->sendOSCBoolMessage("localhost", smileDatectionPort, "/videoFinish", true);
								firstPlayVideo = false;
							}*/
							picture.setAnimation(WaveAnimationEnum, imagesPool->currentUserPhotos.at(currentAniIndex).at(numUserPhotos-1), imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
						

							WaveAnimation *waveAnimation = static_cast<WaveAnimation *>(picture.getAnimation());
							picture.setEnableAnimationTime(director->getCurrentTime());
							waveStartTime = currentTime;
							animationType[currentCenter] = PHOTOwave2SMILE;
						}
						break;
					case PHOTOwave2SMILE:
						{
							//SetCENTERAnimation(picture, imagesPool, i, director);
							picture.setContent(imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
							animationType[currentCenter] = CENTERANI;
						}
						break;
				}
			}
			else {
				Vec2d vec((i % numPicturesInPainting.width) - (currentCenter % numPicturesInPainting.width),
					(i / numPicturesInPainting.width) - (currentCenter / numPicturesInPainting.width));
				switch (animationType[i]) {
				case ANISTATRT:
					{
						if (pictureState[i] == PHOTO2SMILE) {
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
							printf("position center: %d\n", currentCenter);
							assert(false);
						}
						assert(positionDirector > -1);
						//add morphong smile
						SetDirectionMorphAnimation( picture, imagesPool, i, positionDirector, director );
						animationType[i] = CENTER2DIR;
						pictureDirection[i] = positionDirector;
					}
					break;
				case CENTER2DIR:
					{
						picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(pictureDirection[i]).at(0), 
							imagesPool->allLookPhotos.at(i%numLookPeople).at(pictureDirection[i]).at(0));

						LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
						lookAnimation->setCanReverse(true);
						lookAnimation->setProperties(imagesPool->allLookPhotos.at(i%numLookPeople).at(pictureDirection[i]), animation_time);

						int64 timeOffset = static_cast<int64>(getTickFrequency() * max(abs(vec[0]),abs(vec[1])) * animation_timeoffset);
						picture.setEnableAnimationTime(director->getCurrentTime() + timeOffset + static_cast<int64>(rand()%10 * delta * getTickFrequency()));

						animationType[i] = DIRANI;
					}
					break;
				case DIRANI:
					{
						int positionDirector = 0;
						SetDirectionMorphAnimation( picture, imagesPool, i, positionDirector, director );
						animationType[i] = MORPH2CENTER;
						pictureDirection[i] = positionDirector;
					}
					break;
				case MORPH2CENTER:
					{
						//SetCENTERAnimation(picture, imagesPool, i, director);
						picture.setContent(imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
						animationType[i] = CENTERANI;
					}
					break;
				}
			}
		}
		break;
	case INTERACT:
		{
			if (userNo != director->getCurrentVideoNum()-1) {
				//director->sendOSCBoolMessage("localhost", musicPlayerPort, "/event2", true);
				/*director->playMusic(musicPath[musicCounter%4],false, false);
				musicCounter++;*/
				userNo = director->getCurrentVideoNum()-1;
				waitOSC = true;
				waitOSCstartTime = currentTime;
				/*playSmileAnime = true;
				Picture& picture = *director->getPictureAt(22);
				picture.getAnimation()->setEnd(true);
				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setProperties(director->getImagesPool()->currentUserPhotos.at(userNo), videoTime);

				picture.setEnableAnimationTime(director->getCurrentTime());*/
			}
			Picture& lastAddPicture = *director->getPictureAt(currentEyePosIndex);

			if (addFlag) {
				//printf("%d %d\n", lastHandIndex, lastAddPicture.getAnimationSize());
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

			//if (currentEyePosIndex == 17 && eyePosIndex == 8) {
			//	ofstream ofs("eyePos.txt", std::ofstream::app);
			//	if (ofs.good()) {
			//		ofs << eyePos.x << " " << eyePos.y << "\n";
			//	}
			//	else
			//		cerr << "Writing a setting file fails!" << endl;
			//	ofs.close();
			//}

			/*if(currentEyePosIndex == eyePosIndex && (currentTime - starttime) >= eye_stay_time){
				playWave = true;
			} else {
				starttime = currentTime;
			}
			currentEyePosIndex = eyePosIndex;*/
			/*bool switch2end = true;
			for(int i=0; i<numPhotos; i++){
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()  && !(playSmileAnime && i==22)) {
					switch2end = false;
					break;
				}
			}*/
			if (currentTime - startTime > play_time) {
				for(int i=0; i<numPhotos; i++){
					Picture& picture = *director->getPictureAt(i);
					picture.getAnimation()->setEnd(true);
					animationType[i] = ANISTATRT;
				}
				startEndVideoTime = currentTime;
				director->sendOSCBoolMessage("localhost", smileDatectionPort, "/interaction", false);
				//director->sendTopUserVideo();
				broadcastStep = ENDVIDEO;
				break;
			}


			for(int i=0; i<numPhotos; i++){
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}
				SetCENTERAnimation(picture, imagesPool, i, director);
			}

			Picture& picture = *director->getPictureAt(currentEyePosIndex);
			picture.addAnimation(ChangeBackgroundColorAnimationEnum, photo, photo);
			ChangeBackgroundColorAnimation *cbcAnimation = static_cast<ChangeBackgroundColorAnimation *>(picture.getLastAnimation());
			if (currentTime - starttime > 0) {
				cbcAnimation->setProperty((int)(((currentTime - starttime) / eye_stay_time)*cbc_frame));
			} else {
				cbcAnimation->setProperty(0);
			}
			addFlag = true;

			if (playWave || broadcastStep == ENDVIDEO) {
				currentCenter = currentEyePosIndex;
				Picture& picture = *director->getPictureAt(currentCenter);
				picture.getAnimation()->setEnd(true);
				Animation *lastAddAnimation = lastAddPicture.getLastAnimation();
				lastAddAnimation->setEnd(true);
				lastAddPicture.popAnimation();
				addFlag = false;
				broadcastStep = READYTOANI;
			}
		}
		break;
	case READYTOANI:
		{
			if (userNo != director->getCurrentVideoNum()-1) {
				//director->sendOSCBoolMessage("localhost", musicPlayerPort, "/event2", true);
				//director->playMusic(bellPath,false, false);
				/*director->playMusic(musicPath[musicCounter%4],false, false);
				musicCounter++;*/
				userNo = director->getCurrentVideoNum()-1;
				waitOSC = true;
				waitOSCstartTime = currentTime;
				/*playSmileAnime = true;
				Picture& picture = *director->getPictureAt(22);
				picture.getAnimation()->setEnd(true);
				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setProperties(director->getImagesPool()->currentUserPhotos.at(userNo), videoTime);

				picture.setEnableAnimationTime(director->getCurrentTime());
				animationType[22] = ANISTATRT;*/
			}

			for(int i=0; i<numPhotos; i++){
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}
				if (i == currentCenter) {
					picture.setAnimation(WaveAnimationEnum, imagesPool->currentUserPhotos.at(currentAniIndex).at(0), imagesPool->currentUserPhotos.at(currentAniIndex).at(0));


					WaveAnimation *waveAnimation = static_cast<WaveAnimation *>(picture.getAnimation());
					picture.setEnableAnimationTime(director->getCurrentTime());
					animationType[i] = ANISTATRT;
					continue;
				}
				switch (animationType[i]) {
				case MORPH2NEU:
					picture.setContent(imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(numLookNeu-1));
					animationType[i] = ANISTATRT;
					break;
				case ANISTATRT:
					break;
				default:
					{
						int positionDirector = 0;
						SetDirectionMorphAnimation( picture, imagesPool, i, positionDirector, director );

						animationType[i] = MORPH2NEU;
						pictureDirection[i] = positionDirector;
					}
					break;
				}
			}
			bool switch2animation = true;
			for(int i=0; i<numPhotos; i++){
				if (animationType[i] != ANISTATRT){//  && !(playSmileAnime && i==22)) {
					switch2animation = false;
					break;
				}
			}

			if (switch2animation) {
				Picture& picture = *director->getPictureAt(currentCenter);
				picture.getAnimation()->setEnd(true);
				broadcastStep = ANIMATION;
			}
		}
		break;
	case ENDVIDEO:
		{
			if (currentTime - startEndVideoTime > end_video_time) {
				//director->sendTopUserVideo();
				broadcastStep = ENDBUDDHA;
			}
			/*if (playSmileAnime) {
				Picture& picture = *director->getPictureAt(22);
				picture.getAnimation()->setEnd(true);
				playSmileAnime = false;
			}*/
			
			for(int i=0; i<numPhotos; i++){
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}

				for (int j=0; j < lookIndexVector.size(); j++) {
					Vec2d vec((i % numPicturesInPainting.width) - (lookIndexVector[j] % numPicturesInPainting.width),
						(i / numPicturesInPainting.width) - (lookIndexVector[j] / numPicturesInPainting.width));
					if (abs(vec[0]) + abs(vec[1]) < smallestDis[i]) {
						smallestDisIndex[i] = j;
						smallestDis[i] = abs(vec[0]) + abs(vec[1]);
					}
				}
				if (animationType[i] == WAVE2VIDEO || animationType[i] == PHOTOVIDEO) {
					picture.setAnimation(LookAnimationEnum, picture);

					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					lookAnimation->setProperties(director->getImagesPool()->currentUserPhotos.at(pictureAnimationIndex[i]), videoTime);
					lookAnimation->setCanReverse(true);

					picture.setEnableAnimationTime(director->getCurrentTime());	
					animationType[i] = PHOTOVIDEO;
				} else {
					int rdm = i % userNo;
					if (smallestDis[i] == 0) {
						
						pictureAnimationIndex[i] = (end_video_table[i]-1)%director->getCurrentVideoNum();
						picture.setAnimation(LookAnimationEnum, picture);
						LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
						lookAnimation->setProperties(director->getImagesPool()->currentUserPhotos.at(pictureAnimationIndex[i]), videoTime);
						lookAnimation->setCanReverse(true);
						
						picture.addAnimation(NewWaveAnimationEnum, imagesPool->currentUserPhotos.at(pictureAnimationIndex[i]).at(0), imagesPool->currentUserPhotos.at(pictureAnimationIndex[i]).at(0));

						//WaveAnimation *waveAnimation = static_cast<WaveAnimation *>(picture.getAnimation(1));
						int64 timeOffset = static_cast<int64>(getTickFrequency() * (log(static_cast<float>(0)) + abs(randNumGenerator.gaussian(3.0))));
						picture.setEnableAnimationTime(director->getCurrentTime()+timeOffset);
						animationType[i] = WAVE2VIDEO;
					} else {
						pictureAnimationIndex[i] = (end_video_table[i]-1)%director->getCurrentVideoNum();
						picture.setAnimation(LookAnimationEnum, picture);
						LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
						lookAnimation->setProperties(director->getImagesPool()->currentUserPhotos.at(pictureAnimationIndex[i]), videoTime);
						lookAnimation->setCanReverse(true);
						picture.addAnimation(NewWaveAnimationEnum, imagesPool->currentUserPhotos.at(pictureAnimationIndex[i]).at(0), imagesPool->currentUserPhotos.at(pictureAnimationIndex[i]).at(0));

						//WaveAnimation *waveAnimation = static_cast<WaveAnimation *>(picture.getAnimation(1));
						int64 timeOffset = static_cast<int64>(getTickFrequency() * (log(static_cast<float>(smallestDis[i])) + abs(randNumGenerator.gaussian(3.0))));
						picture.setEnableAnimationTime(director->getCurrentTime() + timeOffset);
						animationType[i] = WAVE2VIDEO;
					}
				}
			}
		}
		break;
	case ENDBUDDHA:
		{
			if (currentTime - startEndVideoTime > end_black_time || blackFlag) {
				broadcastStep = ENDBLACK;
				director->sendOSCBoolMessage("localhost", musicPlayerPort, "/fade23", true);
				break;
			}
			Picture& picture = *director->getPictureAt(22);
			Rect centerRect = picture.getFrame();
			Point centerPoint = Point(centerRect.x+centerRect.width/2, centerRect.y+centerRect.height/2);

			int buddhaIndex = rand() % 4 + 1;
			for(int i=0; i<numPhotos; i++){
				Picture& picture = *director->getPictureAt(i);
				if (animationType[i] != WAVE2BUDDHA) {
					//pictureAnimationIndex[i] = rand()%userNo;
					picture.setAnimation(WaveAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), imagesPool->buddhaPhotos.at(buddhaIndex));
					Rect picRect = picture.getFrame();
					Point picPoint = Point(picRect.x+picRect.width/2, picRect.y+picRect.height/2);
					smallestDis[i] = (abs(picPoint.x - centerPoint.x) +  abs(picPoint.y - centerPoint.y))/100;
					int64 timeOffset = static_cast<int64>(getTickFrequency() * (log(static_cast<float>(smallestDis[i]))));
					picture.setEnableAnimationTime(director->getCurrentTime() + timeOffset);
					animationType[i] = WAVE2BUDDHA;
				}
			}
		}
		break;
	case ENDBLACK:
		for (int i=0; i<numPhotos; i++) {
			if (animationType[i] != TOBLACK) {
				Picture& picture = *director->getPictureAt(i);
				picture.getAnimation()->setEnd(true);
				/*picture.setAnimation(FadeAnimationEnum, imagesPool->buddhaPhotos[0]);

				FadeAnimation *fadeAnimation = static_cast<FadeAnimation *>(picture.getAnimation());
				fadeAnimation->setProperties(FADE_IN);*/
				picture.setAnimation(ExposureAnimationEnum, picture.clone());

				ExposureAnimation *exposureeAnimation = static_cast<ExposureAnimation *>(picture.getAnimation());
				exposureeAnimation->setProperties(pic2exp, 5, 0, 350);
				int64 timeOffset = static_cast<int64>(getTickFrequency() * (log(static_cast<float>(smallestDis[i]))));
				picture.setEnableAnimationTime(director->getCurrentTime() + timeOffset);
				animationType[i] = TOBLACK;
			}
		}	
		break;
	}
}

void BroadcastState:: SetDirectionMorphAnimation( Picture &picture, ImagesPool * imagesPool, int i, int positionDirector, Director * director ) 
{
	picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), 
		imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));
	if (pictureDirection[i] != positionDirector) {
		LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
		lookAnimation->setCanReverse(false);
		int smoothPair[2] = {0, 0};
		bool reverse = false;
		if (pictureDirection[i] < positionDirector) {
			smoothPair[0] = pictureDirection[i];
			smoothPair[1] = positionDirector;
		} else {
			smoothPair[1] = pictureDirection[i];
			smoothPair[0] = positionDirector;
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
	}
}

void BroadcastState::SetCENTERAnimation( Picture &picture, ImagesPool * imagesPool, int i, Director * director ) 
{
	picture.setAnimation(LookAnimationEnum, imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0), 
		imagesPool->allLookPhotos.at(i%numLookPeople).at(0).at(0));

	LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
	lookAnimation->setCanReverse(true);
	lookAnimation->setProperties(imagesPool->allLookPhotos.at(i%numLookPeople).at(0));
	picture.setEnableAnimationTime(director->getCurrentTime());
	pictureDirection[i] = 0;
}