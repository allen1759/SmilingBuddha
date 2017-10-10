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
	layout.resize(WINDOW_ROW_COUNT * WINDOW_COL_COUNT);

	// Read layout file.
	std::fstream layoutFile(layoutPath, std::ios::in);
	if (layoutFile.is_open()) {
		for (int i = 0; i < layout.size(); ++i)
			layoutFile >> layout[i].x >> layout[i].y;
	}
	else {
		float gridSizeX = GetResolutionWidth() / static_cast<float>(WINDOW_COL_COUNT);
		float gridSizeY = GetResolutionHeight() / static_cast<float>(WINDOW_ROW_COUNT);
		for (int i = 0; i < WINDOW_ROW_COUNT; ++i) {
			for (int j = 0; j < WINDOW_COL_COUNT; ++j)
				layout[i * WINDOW_COL_COUNT + j] = cv::Point(static_cast<int>(gridSizeX * (float)j), static_cast<int>(gridSizeY * (float)i));
		}
	}
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
	return ROW_CENTER;
}

int Setting::GetCenterCol()
{
	return COL_CENTER;
}

int Setting::GetMaxDistanceToCenterInGrid()
{
	return INTRO_STATE_GRID_WIDTH / 2;
}

int Setting::GetMaxDistanceToCenter()
{
	int ret = 0;
	ret = std::max(ROW_CENTER, std::max(WINDOW_ROW_COUNT - ROW_CENTER - 1, ret));
	ret = std::max(COL_CENTER, std::max(WINDOW_COL_COUNT - COL_CENTER - 1, ret));

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

int Setting::GetResolutionWidth()
{
	return RESOLUTION_WIDTH;
}

int Setting::GetResolutionHeight()
{
	return RESOLUTION_HEIGHT;
}

cv::Point Setting::GetForeheadPositionOfGrid(int row, int col)
{
	cv::Point ret = layout[row * WINDOW_COL_COUNT + col];
	ret.y += GetImageHeight() / 3;
	ret.x += GetImageWidth() / 2;

	return ret;
}

float Setting::GetProjectionWidth()
{
	return PROJECTION_WIDTH;
}

float Setting::GetProjectionHeight()
{
	return PROJECTION_HEIGHT;
}

int Setting::CalculateDistanceToCenter(int row, int col)
{
	int dist = 0;
	dist = std::max(dist, std::abs(row - ROW_CENTER));
	dist = std::max(dist, std::abs(col - COL_CENTER));

	return dist;
}

int Setting::CalculateManhattenDistanceToCenter(int row, int col)
{
	return std::abs(row - ROW_CENTER) + std::abs(col - COL_CENTER);
}

bool Setting::IsInIntroStateGrid(int row, int col)
{
	return CalculateDistanceToCenter(row, col) <= GetMaxDistanceToCenterInGrid();
}

void Setting::GetPairRowColInIntroStateGrid(int & row, int & col, int & nearbyRow, int & nearbyCol)
{
	int index = rand() % SQUARE_SIZE;
	row = ROW_CENTER + DIRECTION[index][1];
	col = COL_CENTER + DIRECTION[index][0];


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

