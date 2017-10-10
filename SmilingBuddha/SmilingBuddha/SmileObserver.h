/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_OBSERVER_H
#define _SMILE_OBSERVER_H

#include <memory>
#include <vector>
#include <opencv2/core.hpp>

class SmileObserver : public std::enable_shared_from_this<SmileObserver>
{
public:
	virtual void OnSmile() = 0;
	virtual void OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images) = 0;

};

#endif // !_SMILE_OBSERVER_H