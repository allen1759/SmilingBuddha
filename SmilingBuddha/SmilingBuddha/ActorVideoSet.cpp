/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "ActorVideoSet.h"

#include <boost\filesystem.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "Setting.h"

ActorVideoSet::ActorVideoSet(int order)
	: IMAGE_WIDTH(Setting::GetInstance()->GetImageWidth()),
	  IMAGE_HEIGHT(Setting::GetInstance()->GetImageHeight())
{
	// Read every direction video.
	ReadDirectionVideo(order, CENTER);
	ReadDirectionVideo(order, DOWN);
	ReadDirectionVideo(order, LEFT);
	ReadDirectionVideo(order, LEFT_DOWN);
	ReadDirectionVideo(order, LEFT_UP);
	ReadDirectionVideo(order, NEUTRAL);
	ReadDirectionVideo(order, RANDOM);
	ReadDirectionVideo(order, RIGHT);
	ReadDirectionVideo(order, RIGHT_DOWN);
	ReadDirectionVideo(order, RIGHT_UP);
	ReadDirectionVideo(order, UP);
}

ActorVideoSet::~ActorVideoSet()
{
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> ActorVideoSet::GetDirectionVideo(int direction)
{
	return directionSet[direction];
}

int ActorVideoSet::GetDirectionIndex(int fromRow, int fromCol, int atRow, int atCol)
{
	if (fromRow > atRow) {
		if (fromCol > atCol) {
			return RIGHT_UP;
		}
		else if (fromCol < atCol) {
			return LEFT_UP;
		}
		else {
			return UP;
		}
	}
	else if (fromRow < atRow) {
		if (fromCol > atCol) {
			return RIGHT_DOWN;
		}
		else if (fromCol < atCol) {
			return LEFT_DOWN;
		}
		else {
			return DOWN;
		}
	}
	else {
		if (fromCol > atCol) {
			return RIGHT;
		}
		else if (fromCol < atCol) {
			return LEFT;
		}
		else {
			// TODO: Should return direction in this case???
			return CENTER;
		}
	}
}


void ActorVideoSet::ReadDirectionVideo(int order, int direction)
{
	boost::filesystem::path p(DIRECTION_PATH + "\\" + std::to_string(order) + "\\" + DIRECTION_FOLDER[direction]);

	for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
		if (it->path().extension().string() == FILE_TYPE) {
			if (directionSet[direction] == NULL)
				directionSet[direction] = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

			directionSet[direction]->push_back(ReadImage(it->path().string()));
		}
	}
}

std::shared_ptr<cv::Mat> ActorVideoSet::ReadImage(const std::string &path)
{
	cv::Mat cacheMat = cv::imread(path);
 	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
	cv::resize(cacheMat, *dst, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	return dst;
}


