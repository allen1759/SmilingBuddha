/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_GRID_H
#define _VIDEO_GRID_H

#include "Video.h"

#include <vector>
#include <memory>
#include <mutex>

#include <opencv2\core\core.hpp>

class VideoGrid : public Video, public std::enable_shared_from_this<VideoGrid>
{
public:
	VideoGrid();

	virtual ~VideoGrid();

	virtual std::shared_ptr<cv::Mat> GetFrame() override;

	virtual std::shared_ptr<Video> GetVideo() override;

	virtual std::shared_ptr<Video> GetChild(int row, int col) override;

	virtual void SetChild(std::shared_ptr<Video> video, int row, int col) override;

private:

	std::vector<std::shared_ptr<Video>> videoGrid;
	std::vector<cv::Point> &layout;
	std::mutex videoGridMutex;

	int rowCount;
	int colCount;


};
#endif // !_VIDEO_GRID_H
