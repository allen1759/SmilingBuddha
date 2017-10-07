/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SETTING_H
#define _SETTING_H

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

	int GetImageSequenceLength();

	int GetActorIndex(int row, int col);

	int GetCenterRow();
	int GetCenterCol();
	//int GetIntroStateGridWidth();
	int GetMaxDistanceToCenterInGrid();
	int GetMaxDistanceToCenter();

	int GetImageWidth();
	int GetImageHeight();

	int GetSaveImageWidth();
	int GetSaveIMageHeight();

	int GetResolutionWidth();
	int GetResolutionHeight();

	float GetProjectionWidth();
	float GetProjectionHeight();

	int CalculateDistanceToCenter(int row, int col);
	int CalculateManhattenDistanceToCenter(int row, int col);
	bool IsInIntroStateGrid(int row, int col);

	void GetPairRowColInIntroStateGrid(int &row, int &col, int &nearbyRow, int &nearbyCol);


private:


	static Setting *instance;

	static const int WINDOW_ROW_COUNT = 4;
	static const int WINDOW_COL_COUNT = 9;
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

	// TODO: read from file.
	const float PROJECTION_WIDTH = 1.92f;
	const float PROJECTION_HEIGHT = 1.08f;
};


#endif // !_SETTING_H
