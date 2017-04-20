/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef ALL_ANIMATIONS_H
#define ALL_ANIMATIONS_H

#include "IdleAnimation.h"
#include "FlipAnimation.h"
#include "FadeAnimation.h"
#include "WaveAnimation.h"
#include "ExposureAnimation.h"

#include "SmilingAnimation.h"
#include "LookAnimation.h"
#include "AddAnimation.h"
#include "NewWaveAnimation.h"
#include "ChangeBackgroundColorAnimation.h"
#include "ScaleAnimation.h"
#include "LookWaveAnimation.h" 

/*!
 * \brief
 * the enumeration of animation.
 * 
 * If you create a new animation, you must add it to the enumeration and 
 an animation factory create the animation.
 * 
 */
typedef enum AnimationEnum {
	IdleAnimationEnum = 0,
	FlipAnimationEnum = 1,
	FadeAnimationEnum = 2,
	WaveAnimationEnum = 3,
	ExposureAnimationEnum = 4,
	SmilingAnimationEnum = 5,
	LookAnimationEnum = 6,
	AddAnimationEnum = 7,
	NewWaveAnimationEnum = 8,
	ChangeBackgroundColorAnimationEnum = 9,
	ScaleAnimationEnum = 10,
	LookWaveAnimationEnum = 11
} AnimationEnum;

#endif // ALL_ANIMATIONS_H