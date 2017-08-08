/*!
*\author YuShan Lin
* 
*\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
*/
#include "AllAnimationStates.h"
#include <iostream>

using namespace std;
using namespace cv;

const double STATE_BUDDHA_COUNTER_TIME = 13.0;
const double STATE_WAVE_COUNTER_TIME = 8.0;
const double WAVE_VERTICAL_COUNTER_TIME = 0.4;
const double WAVE_HORIZONTAL_COUNTER_TIME = 0.8;
const double WAVE_RANDOM_COUNTER_TIME = 0.1;

void PreludeState::processTime(Director *director, const int64 &currentTickCount)
{
	currentTime = currentTickCount/ getTickFrequency();
	if (currentTime - prevTime > counterTime) {
		if(preludeStep == PRE_FADE_IN) {
			buddhaIndex = rand() % 4 + 1;
			preludeStep = PRE_BUDDHA_TO_RANDOM;
		}
		else if (preludeStep == PRE_RANDOM) {
			if(buddhaCounter > 3)
				preludeStep = PRE_RANDOM_TO_BUDDHA;
			else {
				director->sendOSCBoolMessage("localhost", musicPlayerPort, "/bell", true);
				counterTime = STATE_BUDDHA_COUNTER_TIME;
				buddhaCounter++;
				randomIndex = rand() % 36;
				for (int i = 0; i < 36; ++i)
					transitionType[i] = TRANSITION_VIDEO;
			}
		}
		else if(preludeStep == PRE_RANDOM_TO_BUDDHA) {
			buddhaIndex = rand() % 4 + 1;
			counterTime = STATE_WAVE_COUNTER_TIME;
			preludeStep = PRE_BUDDHA_TO_RANDOM;
		}
		else if(preludeStep == PRE_BUDDHA_TO_RANDOM) {
			buddhaCounter = 0;
			director->sendOSCBoolMessage("localhost", musicPlayerPort, "/bell", true);
			randomIndex = rand() % 36;
			counterTime = STATE_BUDDHA_COUNTER_TIME;
			preludeStep = PRE_RANDOM;
		}

		if(preludeStep != PRE_RANDOM) {
			if (waveType == WAVE_RANDOM)
				waveType = WAVE_TO_BOTTOM;
			else if (waveType == WAVE_TO_BOTTOM)
				waveType = WAVE_TO_LEFT;
			else if (waveType == WAVE_TO_LEFT)
				waveType = WAVE_TO_RIGHT;
			else if (waveType == WAVE_TO_RIGHT)
				waveType = WAVE_TO_TOP;
			else if (waveType == WAVE_TO_TOP)
				waveType = WAVE_RANDOM;
			waveCounter = -1;
		}

		prevTime = currentTime;
	}
}

void PreludeState::processKeyEvent(Director *director, const int &key)
{
	//#if SIMULATOR == 1
	
	if (key == 's') {
		preludeStep = PRE_FADE_OUT;
	}
	/*else if (key == 'v') {
		director->setAnimationState(new ViewState, true);
	}
	else if (key == 't') {
		director->takeVideoFromSmileDetect();
	}
	*/
	//#endif
}

void PreludeState::processMouseEvent(Director *director, const Point &mousePos)
{
}

void PreludeState::processOSC(Director *director, OSCListener *oscListener)
{
	if (isFirstSit && oscListener->interactionState == INTERACTION_START) {
		cout << "prelude end" << endl;
		preludeStep = PRE_FADE_OUT;
		isFirstSit = false;
		/*ofstream timeFile("C:/SmileVideo2/time.txt", ofstream::app);
		timeFile << "SITDOWN: " << double(director->getCurrentTime())/getTickFrequency() << endl;
		timeFile.close();*/
		//director->sendOSCBoolMessage("localhost", smileDatectionPort, "/interaction", true);
	}
}

void PreludeState::processAnimation(Director *director)
{
	switch (preludeStep) {
	case PRE_FADE_IN:
		for (int i = 0; i < numPhotos; i++) {
			if (transitionType[i] != TRANSITION_INIT) {
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded())
					continue;
				Mat whiteImage(1, 1, CV_8UC3, Scalar(255, 255, 255));

				picture.setAnimation(FadeAnimationEnum, whiteImage, director->getImagesPool()->buddhaPhotos.at(buddhaIndex));

				FadeAnimation *fadeAnimation = static_cast<FadeAnimation *>(picture.getAnimation());
				fadeAnimation->setProperties(FADE_TO);

				picture.setEnableAnimationTime(director->getCurrentTime());
				transitionType[i] = TRANSITION_INIT;
			}
		}

		break;
	case PRE_RANDOM:
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			double videoStartTime = -1;

			if (transitionType[i] != TRANSITION_SCALE_BUDDHA) {
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
					videoStartTime = lookAnimation->getStartTime();
				}

				picture.setAnimation(ScaleAnimationEnum, picture, director->getImagesPool()->buddhaPhotos.at(0));

				ScaleAnimation *scaleAnimation = static_cast<ScaleAnimation* >(picture.getAnimation());
				scaleAnimation->setProperties(director->getImagesPool()->smileVideoPhotos.at(i%director->getImagesPool()->smileVideoPhotos.size()), 3.0, i/9, i%9, randomIndex/9, randomIndex%9, 0, 4, 0.09, videoStartTime);

				picture.setEnableAnimationTime(director->getCurrentTime());
				transitionType[i] = TRANSITION_SCALE_BUDDHA;
			}
			else {
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}

				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setCanReverse(true);
				lookAnimation->setProperties(director->getImagesPool()->smileVideoPhotos.at(i%director->getImagesPool()->smileVideoPhotos.size()), 3.0f);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}
		}

		break;
	case PRE_RANDOM_TO_BUDDHA:
		double waveCounterTime;
		if (waveType == WAVE_TO_LEFT || waveType == WAVE_TO_RIGHT)
			waveCounterTime = WAVE_VERTICAL_COUNTER_TIME;
		else if (waveType == WAVE_TO_TOP || waveType == WAVE_TO_BOTTOM)
			waveCounterTime = WAVE_HORIZONTAL_COUNTER_TIME;
		else if (waveType == WAVE_RANDOM)
			waveCounterTime = WAVE_RANDOM_COUNTER_TIME;

		if (waveCurrentTime - wavePrevTime > waveCounterTime) {
			waveCounter++;
			wavePrevTime = waveCurrentTime;
		}
		waveCurrentTime = director->getCurrentTime()/getTickFrequency();
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);

			int waveIndex;
			if (waveType == WAVE_TO_LEFT)
				waveIndex = i % 9;
			else if (waveType == WAVE_TO_RIGHT)
				waveIndex = 9 - i % 9;
			else if (waveType == WAVE_TO_TOP)
				waveIndex = i / 9;
			else if (waveType == WAVE_TO_BOTTOM)
				waveIndex = 4 - i / 9;
			else if (waveType == WAVE_RANDOM)
				waveIndex = transitionTable[i];

			if (transitionType[i] != TRANSITION_WAVE_TO_BUDDHA && waveIndex == waveCounter) {
				picture.setAnimation(WaveAnimationEnum, picture, director->getImagesPool()->buddhaPhotos.at(buddhaIndex));
				picture.setEnableAnimationTime(director->getCurrentTime());
				transitionType[i] = TRANSITION_WAVE_TO_BUDDHA;
			}
			else if (transitionType[i] != TRANSITION_WAVE_TO_BUDDHA) {
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}
				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setCanReverse(true);
				lookAnimation->setProperties(director->getImagesPool()->smileVideoPhotos.at(i%director->getImagesPool()->smileVideoPhotos.size()), 3.0f);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}
		}

		break;

	case PRE_BUDDHA_TO_RANDOM:
		if (waveType == WAVE_TO_LEFT || waveType == WAVE_TO_RIGHT)
			waveCounterTime = WAVE_VERTICAL_COUNTER_TIME;
		else if (waveType == WAVE_TO_TOP || waveType == WAVE_TO_BOTTOM)
			waveCounterTime = WAVE_HORIZONTAL_COUNTER_TIME;
		else if (waveType == WAVE_RANDOM)
			waveCounterTime = WAVE_RANDOM_COUNTER_TIME;

		if (waveCurrentTime - wavePrevTime > waveCounterTime) {
			waveCounter++;
			wavePrevTime = waveCurrentTime;
		}
		waveCurrentTime = director->getCurrentTime()/getTickFrequency();

		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);

			int waveIndex;
			if (waveType == WAVE_TO_LEFT)
				waveIndex = i % 9;
			else if (waveType == WAVE_TO_RIGHT)
				waveIndex = 9 -i % 9;
			else if (waveType == WAVE_TO_TOP)
				waveIndex = i / 9;
			else if (waveType == WAVE_TO_BOTTOM)
				waveIndex = 4 - i / 9;
			else if (waveType == WAVE_RANDOM)
				waveIndex = transitionTable[i];

			if (transitionType[i] != TRANSITION_WAVE_TO_VIDEO && waveIndex == waveCounter) {
				cout << i%director->getImagesPool()->smileVideoPhotos.size() << endl;
				picture.setAnimation(LookWaveAnimationEnum, picture, director->getImagesPool()->smileVideoPhotos.at(i%director->getImagesPool()->smileVideoPhotos.size()).at(0));
				LookWaveAnimation *lookWaveAnimation = static_cast<LookWaveAnimation *>(picture.getAnimation());
				lookWaveAnimation->setCanReverse(true);
				lookWaveAnimation->setProperties(director->getImagesPool()->smileVideoPhotos.at(i%director->getImagesPool()->smileVideoPhotos.size()), 3.0f);
				picture.setEnableAnimationTime(director->getCurrentTime());
				transitionType[i] = TRANSITION_WAVE_TO_VIDEO;
			}
			else if (transitionType[i] == TRANSITION_WAVE_TO_VIDEO) {
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}
				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setCanReverse(true);
				lookAnimation->setProperties(director->getImagesPool()->smileVideoPhotos.at(i%director->getImagesPool()->smileVideoPhotos.size()), 3.0f);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}
		}

		break;
	case PRE_FADE_OUT:
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			Mat content;
			picture.copyTo(content);
			picture.setAnimation(FadeAnimationEnum, content);

			FadeAnimation *fadeAnimation = static_cast<FadeAnimation* >(picture.getAnimation());
			fadeAnimation->setProperties(FADE_OUT);

			picture.setEnableAnimationTime(director->getCurrentTime());
		}
		preludeStep = PRE_END;

		break;
	default:
		if (!director->isAllAnimationEnd())
			return;

		//director->setAnimationState(new EndingState, true);
		//director->sendOSCBoolMessage("localhost", smileDatectionPort, "/interaction", true);
		director->sendOSCBoolMessage("localhost", musicPlayerPort, "/fade1", true);
		director->sendOSCBoolMessage("localhost", musicPlayerPort, "/play2", true);
		//director->playMusic("Musics/loopA.wav");
		director->getImagesPool()->currentUserPhotos.clear();
		director->setCurrentVideoNum(0);
		director->setAnimationState(new SeeEachState, true);
		//director->setAnimationState(new BroadcastState, true);
		director->setStartTickCount();
		break;
	}
}
