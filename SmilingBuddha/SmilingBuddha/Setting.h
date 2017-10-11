/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SETTING_H
#define _SETTING_H

#include <vector>

#include <opencv2\core\core.hpp>

#include <fstream>

class Setting
{
private:
	Setting();

public:
	~Setting();

	static Setting * GetInstance();

	int GetRow();
	int GetCol();
	int GetWindowCount();

	int GetCenterRow();
	int GetCenterCol();
	int GetMaxDistanceToCenterInGrid();
	int GetMaxDistanceToCenter();
	void GetPairRowColInIntroStateGrid(int &row, int &col, int &nearbyRow, int &nearbyCol);

	int CalculateDistanceToCenter(int row, int col);
	int CalculateManhattenDistanceToCenter(int row, int col);
	bool IsInIntroStateGrid(int row, int col);

	int GetActorIndex(int row, int col);
	int GetImageSequenceLength();
	int GetImageWidth();
	int GetImageHeight();
	int GetSaveImageWidth();
	int GetSaveIMageHeight();

	int GetResolutionWidth();
	int GetResolutionHeight();

	int GetCameraIndex();

	cv::Point GetForeheadPositionOfGrid(int row, int col);


	float GetProjectionWidth();
	float GetProjectionHeight();

	std::string GetComport();

	const std::vector<cv::Point> &GetLayout();

	void SetLayout(int row, int col, cv::Point point);

	void SaveLayout();

	const cv::Rect &GetDetectRoi()
	{
		return DETECT_ROI;
	}

	void SetDetectRoi(cv::Rect newRoi);

	void SaveDetectRoi();

private:


	static Setting *instance;

	static const int ROW_COUNT = 4;
	static const int COL_COUNT = 9;
	static const int IMAGE_SEQUENCE_LENGTH = 40;

	const int ACTOR_INDEX[36] = {27, 3, 37, 9, 24, 35, 1, 2, 6,
							31, 10, 25, 34, 4, 13, 11, 16, 0,
							28, 19, 32, 26, 20, 7, 12, 23, 17,
							5, 33, 15, 8, 14, 36, 30, 29, 21};

	static const int ROW_CENTER = 2;
	static const int COL_CENTER = 4;
	static const int INTRO_STATE_GRID_WIDTH = 3;

	static const int SQUARE_SIZE = 9;
	const int DIRECTION[9][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
								{ 0, -1 },{ 0, 0 },{ 0, 1 },
								{ 1, -1 },{ 1, 0 },{ 1, 1 } };
	const int NEAR_BY_DIRECTION[8][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
										{ 0, -1 },{ 0, 1 },
										{ 1, -1 },{ 1, 0 },{ 1, 1 } };

	static const int IMAGE_WIDTH = 130;
	static const int IMAGE_HEIGHT = 177;
	static const int SAVE_IMAGE_WIDTH = 260;
	static const int SAVE_IMAGE_HEIGHT = 355;

	static const int RESOLUTION_WIDTH = 1920;
	static const int RESOLUTION_HEIGHT = 1080;

	static const int CAMERA_INDEX = 0;

	// Projection width, height on the projection plane.
	float PROJECTION_WIDTH = 1.92f;
	float PROJECTION_HEIGHT = 1.08f;
	std::string projectionParametersPath = "resources\\settings\\projection_parameters.txt";

	// Comport of Arduino
	std::string COMPORT = "COM1";
	std::string comportPath = "resources\\settings\\comport.txt";

	// Layout of grids.
	std::vector<cv::Point> layout;
	std::string layoutPath = "resources\\settings\\layout.txt";

	// Rect of ROI
	cv::Rect DETECT_ROI;
	std::string detectRoiPath = "resources\\settings\\detect_roi.txt";
};


#endif // !_SETTING_H
