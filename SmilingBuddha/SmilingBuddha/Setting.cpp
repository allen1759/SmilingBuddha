/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Setting.h"

#include <iostream>

Setting *Setting::instance = NULL;

Setting::Setting()
{
}

Setting::~Setting()
{
}

Setting * Setting::GetInstance()
{
	if (instance == NULL)
		instance = new Setting();

	return instance;
}

int Setting::GetRow()
{
	return WINDOW_ROW_COUNT;
}

int Setting::GetCol()
{
	return WINDOW_COL_COUNT;
}

int Setting::GetImageSequenceLength()
{
	return IMAGE_SEQUENCE_LENGTH;
}

int Setting::GetActorIndex(int row, int col)
{
	return ACTOR_INDEX[row * WINDOW_COL_COUNT + col];
}

int Setting::GetCenterRow()
{
	return CENTER_ROW;
}

int Setting::GetCenterCol()
{
	return CENTER_COL;
}
