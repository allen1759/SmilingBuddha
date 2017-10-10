/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include <iostream>
#include <vector>
#include <memory>

#include "../../SmilingBuddha/SmilingBuddha/VideoClip.h"
#include "../../SmilingBuddha/SmilingBuddha/FadeAnimation.h"

int main()
{
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

	for (int i = 100; i < 200; ++i) {
		imgs->push_back(std::make_shared<cv::Mat>(cv::Mat(500, 500, CV_8UC3, cv::Scalar(100, 0, i))));
	}

	std::shared_ptr<Video> video = std::make_shared<VideoClip>(imgs, 10.0f);
	video = std::make_shared<FadeAnimation>(video, 5.0f);

	while (true) {
		cv::imshow("window", *video->GetFrame());
		cv::waitKey(1);
	}

	return 0;
}