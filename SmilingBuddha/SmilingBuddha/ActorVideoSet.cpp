/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "ActorVideoSet.h"

ActorVideoSet::ActorVideoSet(int order)
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


	// Read morphing video in every direction.
	//ReadMorphingVideo(order);
}

ActorVideoSet::~ActorVideoSet()
{
}

std::shared_ptr<const std::vector<std::shared_ptr<cv::Mat>>> ActorVideoSet::GetDirectionVideo(int direction)
{
	return directionSet[direction];
}

//std::shared_ptr<const std::vector<std::shared_ptr<cv::Mat>>> ActorVideoSet::GetMorphingVideo(int index)
//{
//	return morphingSet[index];
//}

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

//void ActorVideoSet::ReadMorphingVideo(int order)
//{
//	for (int i = 1; i <= MORPHING_SIZE; ++i) {
//		boost::filesystem::path p(DIRECTION_PATH + "\\" + std::to_string(order) + "\\0-" + std::to_string(i));
//
//		for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
//			if (it->path().extension().string() == FILE_TYPE) {
//				if (morphingSet[i] == NULL)
//					morphingSet[i] = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
//
//				morphingSet[i]->push_back(ReadImage(it->path().string()));
//			}
//		}
//	}
//}

std::shared_ptr<cv::Mat> ActorVideoSet::ReadImage(const std::string &path)
{
	cv::Mat cacheMat = cv::imread(path);
 	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
	cv::resize(cacheMat, *dst, cv::Size(cacheMat.cols >> 1, cacheMat.rows >> 1));

	return dst;
}


