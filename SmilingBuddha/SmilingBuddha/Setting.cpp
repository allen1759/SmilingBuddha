/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Setting.h"

#include <cstdlib>
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

int Setting::GetWindowCount()
{
	return WINDOW_COL_COUNT * WINDOW_ROW_COUNT;
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

int Setting::GetIntroStateGridWidth()
{
	return INTRO_STATE_GRID_WIDTH;
}

int Setting::GetMaxDistanceToCenterInGrid()
{
	return INTRO_STATE_GRID_WIDTH / 2;
}

int Setting::GetMaxDistanceToCenter()
{
	int ret = 0;
	ret = std::max(CENTER_ROW, std::max(WINDOW_ROW_COUNT - CENTER_ROW - 1, ret));
	ret = std::max(CENTER_COL, std::max(WINDOW_COL_COUNT - CENTER_COL - 1, ret));

	return ret;
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
	return CalculateDistanceToCenter(row, col) <= GetMaxDistanceToCenterInGrid();
}

void Setting::GetPairRowColInIntroStateGrid(int & row, int & col, int & nearbyRow, int & nearbyCol)
{
	GetRandomRowColInIntroStateGrid(row, col);
	GetNearbyRowColInIntroStateGrid(row, col, nearbyRow, nearbyCol);
}

void Setting::GetRandomRowColInIntroStateGrid(int & row, int & col)
{
	int index = rand() % SQUARE_SIZE;
	row = CENTER_ROW + DIRECTION[index][1];
	col = CENTER_COL + DIRECTION[index][0];
}

void Setting::GetNearbyRowColInIntroStateGrid(int row, int col, int & nearbyRow, int & nearbyCol)
{
	if (!IsInIntroStateGrid(row, col))
		return;

	int startIndex = rand() % (SQUARE_SIZE - 1);
	for (int i = 0; i < (SQUARE_SIZE - 1); ++i) {
		int index = (startIndex + i) % (SQUARE_SIZE - 1);
		int anotherRow = row + NEAR_BY_DIRECTION[index][1];
		int anotherCol = col + NEAR_BY_DIRECTION[index][0];

		if (Setting::GetInstance()->IsInIntroStateGrid(anotherRow, anotherCol)) {
			nearbyRow = anotherRow;
			nearbyCol = anotherCol;
			return;
		}
	}
}
