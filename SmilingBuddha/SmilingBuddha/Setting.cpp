/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Setting.h"

#include <algorithm>

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

int Setting::GetImageWidth()
{
	return IMAGE_WIDTH;
}

int Setting::GetImageHeight()
{
	return IMAGE_HEIGHT;
}

int Setting::GetSaveImageWidth()
{
	return SAVE_IMAGE_WIDTH;
}

int Setting::GetSaveIMageHeight()
{
	return SAVE_IMAGE_HEIGHT;
}

int Setting::CalculateDistanceToCenter(int row, int col)
{
	int dist = 0;
	dist = std::max(dist, std::abs(row - CENTER_ROW));
	dist = std::max(dist, std::abs(col - CENTER_COL));

	return dist;
}

bool Setting::IsInIntroStateGrid(int row, int col)
{
	const int MAX_DISTANCE_TO_CENTER = INTRO_STATE_GRID_WIDTH / 2;

	return CalculateDistanceToCenter(row, col) <= MAX_DISTANCE_TO_CENTER;
}
