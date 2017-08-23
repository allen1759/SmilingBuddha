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
	{}
	virtual ~Video()
	{}

public:
	virtual std::shared_ptr<cv::Mat> GetFrame() = 0;

	virtual std::shared_ptr<Video> GetVideo() = 0;

};


#endif // !_VIDEO_H
