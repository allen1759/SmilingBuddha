/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ACTOR_VIDEO_SET_H
#define _ACTOR_VIDEO_SET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace cv
{
	class Mat;
}

class ActorVideoSet
{
public:
	ActorVideoSet(int order);

	~ActorVideoSet();

	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> GetDirectionVideo(int direction);

	static int GetDirectionIndex(int fromRow, int fromCol, int atRow, int atCol);


	// Index of direction.
	const static int NEUTRAL = 0;
	const static int LEFT_DOWN = 1;
	const static int DOWN = 2;
	const static int RIGHT_DOWN = 3;
	const static int LEFT = 4;
	const static int CENTER = 5;
	const static int RIGHT = 6;
	const static int LEFT_UP = 7;
	const static int UP = 8;
	const static int RIGHT_UP = 9;
	const static int RANDOM = 10;

private:

	void ReadDirectionVideo(int order, int direction);

	// Read image from file and resize.
	std::shared_ptr<cv::Mat> ReadImage(const std::string &path);


	const static int DIRECTION_SIZE = 11;

	const std::string DIRECTION_PATH = "D:\\Direction";
	const std::string DIRECTION_FOLDER[DIRECTION_SIZE] = { "Neu", "left_down", "down", "right_down", "left", "center", "right", "left_up", "up", "right_up", "Ran" };
	const std::string FILE_TYPE = ".png";

	const int IMAGE_WIDTH;
	const int IMAGE_HEIGHT;

	std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>> directionSet;
};

#endif // !_ACTOR_VIDEO_SET_H
