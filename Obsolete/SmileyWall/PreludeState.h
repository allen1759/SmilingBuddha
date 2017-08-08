/*!
*\author Zheng-Xiang Ke
* 
*\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
*/
#pragma once
#ifndef PRELUDE_STATE_H
#define PRELUDE_STATE_H

#include "AnimationState.h"
#include "Director.h"
#include <ctime>

typedef enum PreludeStep{
	PRE_FADE_IN,				//!< - PRE_FADE_IN: fade form white to buddha
	PRE_RANDOM,					//!< - PRE_RANDOM: play random photos and wave buddha animation.
	PRE_RANDOM_TO_BUDDHA,	//!< - PRE_RANDOM_TO_BUDDHA
	PRE_BUDDHA_TO_RANDOM,	//!< - PRE_BUDDHA_TO_RANDOM
	PRE_FADE_OUT,				//!< - PRE_FADE_OUT: pictures fade out to start next state.
	PRE_END} PreludeStep;

typedef enum TransitionType{TRANSITION_INIT, TRANSITION_VIDEO, TRANSITION_SCALE_BUDDHA ,TRANSITION_WAVE_TO_BUDDHA, TRANSITION_WAVE_TO_VIDEO} TransitionType;
typedef enum WaveType{WAVE_TO_RIGHT, WAVE_TO_LEFT, WAVE_TO_BOTTOM, WAVE_TO_TOP, WAVE_RANDOM} WaveType;


/*!
* \brief
* Prelude animation state.
* 
* Prelude repeats flip animations from baby, girl to elder.
*/
class PreludeState: public AnimationState
{
public:
	PreludeState(): preludeStep(PRE_FADE_IN), isFirstSit(true) {
		/*int numArray[] = {
		43,35,27,19,9,10,11,12,20,28,36,
		42,34,26,18,8,1,2,13,21,29,37,
		41,33,25,17,7,0,3,14,22,30,38,
		40,32,24,16,6,5,4,15,23,31,39
		};*/

		srand(static_cast<unsigned int>(time(NULL)));

		int numArray[] = {
			30,13,39,5,18,36,19,10,20,3,31,
			8,22,17,27,6,41,24,4,33,14,21,
			37,7,26,40,34,0,12,29,42,1,35,
			9,43,2,11,28,16,32,23,38,25,15
		};

		int randomArray[] = {
			18, 3, 29, 6, 35, 12, 22, 9, 25,
			33, 23, 11, 26, 15, 19, 2, 32, 5,
			8, 30, 20, 34, 1, 24, 28, 16, 36,
			27, 14, 4, 17, 31, 7, 13, 21, 10};

			for (int i=0; i<44; i++) {
				userPhotoArray[i] = numArray[i];
			}

			// shuffle array
			for (int i = 35; i >= 0; --i) {
				int randomNumber = rand() % 36;
				int swapNumber = randomArray[i];
				randomArray[i] = randomArray[randomNumber];
				randomArray[randomNumber] = swapNumber;
			}

			for (int i = 0; i < 36; ++i) {
				transitionTable[i] = randomArray[i];
				transitionType[i] = TRANSITION_VIDEO;
			}

			counterTime = 5.0;
			waveType = WAVE_TO_RIGHT;
			prevTime = getTickCount() / getTickFrequency();
			currentTime = 0.0;
			wavePrevTime = 0.0;
			waveCurrentTime = 0.0;
			waveCounter = -1;
			buddhaCounter = 0;

			randomIndex = rand() % 36;

			buddhaIndex = rand() % 4 + 1;
	}

	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, OSCListener *oscListener);
	void processAnimation(Director *director);

private:
	PreludeStep preludeStep;
	bool isFirstSit;
	int userPhotoArray[44];
	int transitionTable[36];
	TransitionType transitionType[36];
	double prevTime;
	double currentTime;
	double wavePrevTime;
	double waveCurrentTime;
	double counterTime;
	int waveCounter;
	int buddhaCounter;
	WaveType waveType;
	int randomIndex;
	int buddhaIndex;
};

#endif // PRELUDE_STATE_H