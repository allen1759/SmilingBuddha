/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_H
#define _VIDEO_H

#include <memory>
#include <opencv2/core/core.hpp>

#include <iostream>

class Video
{
protected:
	Video()
	{
	}

public:
	virtual ~Video()
	{
	}

	virtual std::shared_ptr<cv::Mat> GetFrame() = 0;

	virtual std::shared_ptr<Video> GetVideo() = 0;

	virtual std::shared_ptr<Video> GetChild(int row, int col)
	{
		return NULL;
	}

	virtual void SetChild(std::shared_ptr<Video> video, int row, int col)
	{
	}
};


#endif // !_VIDEO_H
