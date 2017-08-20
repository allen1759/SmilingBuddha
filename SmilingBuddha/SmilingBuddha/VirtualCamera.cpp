/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VirtualCamera.h"

#include <iostream>

VirtualCamera * VirtualCamera::instance = NULL;

VirtualCamera::VirtualCamera()
{
}

VirtualCamera::~VirtualCamera()
{
}

VirtualCamera * VirtualCamera::GetInstance()
{
	if (instance)
		instance = new VirtualCamera();

	return instance;
}

