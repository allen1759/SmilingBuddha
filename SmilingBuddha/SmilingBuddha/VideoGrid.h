/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_GRID_H
#define _VIDEO_GRID_H

#include "Video.h"

class VideoGrid : public Video
{
public:
	VideoGrid(int rowCount, int colCount);
	~VideoGrid();

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual std::shared_ptr<Video> GetVideo() override;

	Video *GetChild(int row, int col);

	void SetChild(Video *video, int row, int col);

private:
	Video **videoGrid;

	int rowCount;
	int colCount;

};
#endif // !_VIDEO_GRID_H
