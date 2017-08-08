
/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef SEEEACH_STATE_H
#define SEEEACH_STATE_H

#include "AnimationState.h"
#include "ImagesPool.h"
#include <time.h>

typedef enum SeeEachStep{SEEEACH_INITIAL, DETECT_SMILE, PLAYANI, AFTER_SMILE} SeeEachStep;

/*!
 * \brief
 * Interactive animation state.
 * 
 * Show your photo and interact with ZhengMeiXiang.
 */
class SeeEachState: public AnimationState
{
public:
	SeeEachState(): eyePosIndex(22), lastSelectTime(-1), seeEachStep(SEEEACH_INITIAL), selectState(0), userSmile(false), startTime(-1), readedUserPhoto(false), userNo(0), OSCFlag(true) {
		for (int i=0; i<numPhotos; i++) {
			pictureState[i] = -1;
			animationType[i] = ANISTATRT;
		}
		selectPair[0] = -1;
		selectPair[1] = -1;
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
	int userNo;
	int lastSelectTime;
	double firstStayTime;
	std::vector<int> selectIndex;
	SeeEachStep seeEachStep;
	int pictureState[36]; //-1->initial, 0~9->smile type, 10->0-smile
	int selectState; //0->not select, 1->wait neutral end, 2->wait neutral2smile, 3->wait smooth animation, 4->laugh reverse end, 5->wait reverse smooth animation, 6->smile2neutral
	int selectPair[2];
	bool userSmile;
	double startTime;
	bool readedUserPhoto;

	bool aniFinishFlag;
	AnimationType animationType[36];
	bool OSCFlag;

	int SeeEachState::whereOtherSide( int aIndex, int bIndex );
	void selectPairForSeeEach();
	bool isSelectAniEnd( Director * director );
	void SetDirectionMorphAnimation( Picture &picture, ImagesPool * imagesPool, int i, int first, int second, Director * director );
};

#endif // SeeEach_STATE_H