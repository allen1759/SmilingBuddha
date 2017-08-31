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

private:
	static Setting *instance;

	const int WINDOW_ROW_COUNT = 4;
	const int WINDOW_COL_COUNT = 9;
	const int IMAGE_SEQUENCE_LENGTH = 40;
};


#endif // !_SETTING_H
