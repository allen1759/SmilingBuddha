/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Setting.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

Setting *Setting::instance = NULL;

Setting::Setting()
{
	// Read projection parameters.
	std::fstream projectionFile(projectionParametersPath, std::ios::in);
	if (projectionFile.is_open())
		projectionFile >> PROJECTION_WIDTH >> PROJECTION_HEIGHT;
	else
		std::cerr << "Projection file not found." << std::endl;
	projectionFile.close();

	// Read comport file.
	std::fstream comportFile(comportPath, std::ios::in);
	if (comportFile.is_open())
		comportFile >> COMPORT;
	else
		std::cerr << "Comport file not found." << std::endl;
	comportFile.close();

	// Read layout file.
	layout.resize(ROW_COUNT * COL_COUNT);
	std::fstream layoutFile(layoutPath, std::ios::in);
	if (layoutFile.is_open()) {
		for (int i = 0; i < layout.size(); ++i)
			layoutFile >> layout[i].x >> layout[i].y;
	}
	else {
		float gridSizeX = GetResolutionWidth() / static_cast<float>(COL_COUNT);
		float gridSizeY = GetResolutionHeight() / static_cast<float>(ROW_COUNT);
		for (int i = 0; i < ROW_COUNT; ++i) {
			for (int j = 0; j < COL_COUNT; ++j)
				layout[i * COL_COUNT + j] = cv::Point(static_cast<int>(gridSizeX * (float)j), static_cast<int>(gridSizeY * (float)i));
		}
	}
	layoutFile.close();

	// Read detect ROI
	std::fstream detectRoiFile(detectRoiPath, std::ios::in);
	if (detectRoiFile.is_open())
		detectRoiFile >> DETECT_ROI.x >> DETECT_ROI.y >> DETECT_ROI.width >> DETECT_ROI.height;
	else {
		DETECT_ROI.x = 0;
		DETECT_ROI.y = 0;
		DETECT_ROI.width = RESOLUTION_WIDTH;
		DETECT_ROI.height = RESOLUTION_HEIGHT;
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
	return ROW_COUNT;
}

int Setting::GetCol()
{
	return COL_COUNT;
}

int Setting::GetWindowCount()
{
	return COL_COUNT * ROW_COUNT;
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
	ret = std::max(ROW_CENTER, std::max(ROW_COUNT - ROW_CENTER - 1, ret));
	ret = std::max(COL_CENTER, std::max(COL_COUNT - COL_CENTER - 1, ret));

	return ret;
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

int Setting::GetActorIndex(int row, int col)
{
	return ACTOR_INDEX[row * COL_COUNT + col];
}

int Setting::GetImageSequenceLength()
{
	return IMAGE_SEQUENCE_LENGTH;
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

int Setting::GetCameraIndex()
{
	return CAMERA_INDEX;
}

cv::Point Setting::GetForeheadPositionOfGrid(int row, int col)
{
	cv::Point ret = layout[row * COL_COUNT + col];
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

std::string Setting::GetComport()
{
	return COMPORT;
}

const std::vector<cv::Point>& Setting::GetLayout()
{
	return layout;
}

void Setting::SetLayout(int row, int col, cv::Point point)
{
	point.x = std::max(0, point.x);
	point.y = std::max(0, point.y);

	point.x = std::min(RESOLUTION_WIDTH - IMAGE_WIDTH, point.x);
	point.y = std::min(RESOLUTION_HEIGHT - IMAGE_HEIGHT, point.y);

	layout[row * COL_COUNT + col] = point;
}

void Setting::SaveLayout()
{
	std::fstream layoutFile(layoutPath, std::ios::out);
	for (int i = 0; i < layout.size(); ++i)
		layoutFile << layout[i].x << "\t" << layout[i].y << std::endl;

	layoutFile << "// xPosition\tyPosition" << std::endl;
	layoutFile.close();
}

void Setting::SetDetectRoi(cv::Rect newRoi)
{
	newRoi.width = std::min(RESOLUTION_WIDTH, newRoi.width);
	newRoi.height = std::min(RESOLUTION_HEIGHT, newRoi.height);

	newRoi.x = std::max(0, newRoi.x);
	newRoi.y = std::max(0, newRoi.y);

	newRoi.x = std::min(RESOLUTION_WIDTH - newRoi.width, newRoi.x);
	newRoi.y = std::min(RESOLUTION_HEIGHT - newRoi.height, newRoi.y);

	DETECT_ROI = newRoi;
}

void Setting::SaveDetectRoi()
{
	std::fstream detectRoiFile(detectRoiPath, std::ios::out);
	detectRoiFile << DETECT_ROI.x << "\t" << DETECT_ROI.y << "\t"
				  << DETECT_ROI.width << "\t" << DETECT_ROI.height << std::endl;
	detectRoiFile << "//x\ty\twidth\theight" << std::endl;

	detectRoiFile.close();
}
