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
public:
	virtual std::shared_ptr<cv::Mat> GetFrame()
	{
		std::cout << "Video" << std::endl;
		return NULL;
	}
	virtual Video * GetVideo() = 0;

protected:
	Video()
	{
	}
	virtual ~Video()
	{}
};


#endif // !_VIDEO_H
