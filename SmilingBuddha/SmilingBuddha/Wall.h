/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _WALL_H
#define _WALL_H

#include <memory>
#include <vector>

#include "SmileWindow.h"
#include "Video.h"

class Wall
{
public:
	Wall();

	~Wall();

	void Draw();

	void Update();

	std::shared_ptr<Video> GetVideo(int row, int col);

	void SetVideo(std::shared_ptr<Video>, int row, int col);

	static const int WINDOW_ROW_COUNT = 4;
	static const int WINDOW_COL_COUNT = 9;

private:
	std::vector<SmileWindow> windowList;
	std::vector<std::shared_ptr<Video>> videoList;

};

Wall::Wall()
{
}

Wall::~Wall()
{
}

inline void Wall::Draw()
{
	for each (SmileWindow window in windowList)
		window.Draw();
}

inline void Wall::Update()
{
}

inline std::shared_ptr<Video> Wall::GetVideo(int row, int col)
{
	return videoList[row * 9 + col];
}

inline void Wall::SetVideo(std::shared_ptr<Video>, int row, int col)
{
}
#endif // !_WALL_H
