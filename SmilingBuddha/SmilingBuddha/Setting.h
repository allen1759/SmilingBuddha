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

	int GetImageSequenceLength();

	int GetActorIndex(int row, int col);

	int GetCenterRow();
	int GetCenterCol();


private:
	static Setting *instance;

	const int WINDOW_ROW_COUNT = 4;
	const int WINDOW_COL_COUNT = 9;
	const int IMAGE_SEQUENCE_LENGTH = 40;

	const int ACTOR_INDEX[36] = {27, 3, 37, 9, 24, 35, 1, 2, 6,
							31, 10, 25, 34, 4, 13, 11, 16, 0,
							28, 19, 32, 26, 20, 7, 12, 23, 17,
							5, 33, 15, 8, 14, 36, 30, 29, 21};
	const int CENTER_ROW = 2;
	const int CENTER_COL = 4;
};


#endif // !_SETTING_H
