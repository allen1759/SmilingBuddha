
/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef BROADCAST_STATE_H
#define BROADCAST_STATE_H

#include "AnimationState.h"
#include "ImagesPool.h"
#include <time.h>

typedef enum BroadcastStep{BROADCAST_INITIAL, INTERACT, READYTOANI, ANIMATION, ENDVIDEO, ENDBUDDHA, ENDBLACK} BroadcastStep;
//typedef enum AnimationType{ANISTATRT, SMILEwave2PHOTO, PHOTOVIDEO, PHOTOwave2SMILE, MORPH2CENTER, CENTER2DIR, DIRANI, CENTERANI, MORPH2NEU, WAVE2VIDEO, WAVE2BUDDHA, TOBLACK} AnimationType;
typedef enum PictureState{INITIAL, SMILE2SMILE, LAUGH, SMILE2PHOTO, PHOTOSMILE, PHOTO2SMILE} PictureState;

const string musicPath[] = {"Musics/loopD.wav", "Musics/loopE.wav", "Musics/loopF.wav", "Musics/loopG.wav"};

/*!
 * \brief
 * Interactive animation state.
 * 
 * Show your photo and interact with ZhengMeiXiang.
 */
class BroadcastState: public AnimationState
{
public:
	BroadcastState(): eyePosIndex(22), broadcastStep(BROADCAST_INITIAL), currentEyePosIndex(22), currentCenter(22),
		startTime(-1), lastSmileState(false), currentSmileState(false), lastTakePhotoTime(0), takePhotoFlag(false), 
		userNo(0), aniFinishFlag(false), playSmileAnime(false), addFlag(false), musicCounter(0), currentAniIndex(0), 
		blackFlag(false), firstPlayVideo(true), waitOSC(false) {
		for (int i=0; i<numPhotos; i++) {
			pictureDirection[i] = 0;
			pictureState[i] = INITIAL;
			animationType[i] = ANISTATRT;
			newTurn[i] = false;
			smallestDisIndex[i] = 22;
			smallestDis[i] = 11;
			lookIndexVector.push_back(22);
			lookVideoVector.push_back(0);
		}

		for (int i=0; i<5; i++) {
			eyePosIndexArr[i] = -1;
		}
	}
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

	

private:
	int eyePosIndex;
	cv::Mat photo;
	ImagesPool *mp;
	double currentTime;
	int lastEyePosIndex;
	double firstStayTime;
	int pictureDirection[36]; //0~9 direction
	bool newTurn[36];
	PictureState pictureState[36];
	AnimationType animationType[36];
	BroadcastStep broadcastStep;
	double lastTakePhotoTime;
	bool takePhotoFlag;

	int eyePosIndexArr[5];
	int currentEyePosIndex;
	int currentCenter;
	double startTime;
	bool lastSmileState;
	bool currentSmileState;

	double waveStartTime;
	bool aniFinishFlag;
	bool newANI;

	int userNo;
	double startEndVideoTime;
	int pictureAnimationIndex[36]; //video index of ENDVIDEO step
	int smallestDisIndex[36];
	int smallestDis[36];
	double starttime;
	int attpt;
	int isEyeStayMoreThenThreshold;
	int first; //1: new or last time out of threshold/8-neighbor

	std::vector<int> lookIndexVector;
	std::vector<int> lookVideoVector;
	cv::RNG randNumGenerator;
	bool addFlag;
	int musicCounter;
	int currentAniIndex;
	bool blackFlag;

	bool playSmileAnime;
	Point2f eyePos;
	bool firstPlayVideo;
	bool waitOSC;
	double waitOSCstartTime;

	void SetDirectionMorphAnimation( Picture &picture, ImagesPool * imagesPool, int i, int positionDirector, Director * director );
	void SetCENTERAnimation( Picture &picture, ImagesPool * imagesPool, int i, Director * director );
};

#endif // BROADCAST_STATE_H