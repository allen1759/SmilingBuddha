/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoPool.h"

#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

#include "Wall.h"

VideoPool * VideoPool::instance = NULL;

VideoPool::VideoPool()
{
	LoadSlotSmileVideo(Wall::WINDOW_COL_COUNT * Wall::WINDOW_ROW_COUNT);
	LoadNonSlotSmileVideo(Wall::WINDOW_COL_COUNT * Wall::WINDOW_ROW_COUNT);

	LoadAllSmileVideo(Wall::WINDOW_COL_COUNT * Wall::WINDOW_ROW_COUNT);

	//for (int i = 0; i < ACTOR_COUNT; ++i)
	//	actorVideoSets.push_back(std::make_shared<ActorVideoSet>(i));
}

VideoPool::~VideoPool()
{
}

VideoPool * VideoPool::GetInstance()
{
	if (!instance)
		instance = new VideoPool();

	return instance;
}

std::shared_ptr<const std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetDirectionVideo(int row, int col, int direction)
{
	return actorVideoSets[row * Wall::WINDOW_COL_COUNT + col]->GetDirectionVideo(direction);
}

std::shared_ptr<const std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetMorphingVideo(int row, int col, int index)
{
	return actorVideoSets[row * Wall::WINDOW_COL_COUNT + col]->GetMorphingVideo(index);
}

void VideoPool::AddSmileVideo(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> newSmileVideo)
{
	nonslotSmileVideoList.pop_back();
	nonslotSmileVideoList.push_front(newSmileVideo);

	LoadAllSmileVideo(Wall::WINDOW_COL_COUNT * Wall::WINDOW_ROW_COUNT);
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetSmileVideoList(int row, int col)
{
	return smileVideoList[row * Wall::WINDOW_COL_COUNT + col];
}

void VideoPool::LoadSlotSmileVideo(const int windowCount)
{
	boost::filesystem::path p(SLOT_PATH);
	for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
		try {
			if (!it->path().has_extension()) {
				int slotIndex = std::stoi(it->path().filename().string());
				if (slotIndex < windowCount) {
					slotSmileVideoSet[slotIndex] = LoadOneSlotVideo(it->path().string());
				}
			}
		}
		catch (const std::exception &e) {
			std::cerr << "Load Slot SmileVideo Error: " << e.what() << std::endl;
		}
	}
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::LoadOneSlotVideo(const std::string path)
{
	boost::filesystem::path p(path);
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

	for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
		if (it->path().extension().string() == FILE_TYPE)
			imgs->push_back(ReadImage(it->path().string()));
	}

	if (imgs->size() != IMAGE_SEQUENCE_LENGTH)
		return NULL;
	return imgs;
}

void VideoPool::LoadNonSlotSmileVideo(const int windowCount)
{
	int daysAgo = 0;
	std::shared_ptr<std::list<std::string>> smileVideoPathList = NULL;

	while (nonslotSmileVideoList.size() < windowCount) {
		while (smileVideoPathList == NULL || smileVideoPathList->empty()) {
			smileVideoPathList = GetLastSmileVideoPath(daysAgo);
			daysAgo++;
		}

		std::string path = smileVideoPathList->back();
		smileVideoPathList->pop_back();

		std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
		boost::filesystem::path p(path);
		for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
			if (it->path().extension().string() == FILE_TYPE)
				imgs->push_back(ReadImage(it->path().string()));
		}

		if (imgs->size() == IMAGE_SEQUENCE_LENGTH)
			nonslotSmileVideoList.push_back(imgs);
	}
}

void VideoPool::LoadAllSmileVideo(const int windowCount)
{
	std::list<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>::iterator it = nonslotSmileVideoList.begin();

	for (int i = 0; i < windowCount; ++i) {
		if (slotSmileVideoSet[i] == NULL) {
			smileVideoList.push_back(*it);
			it++;
		}
		else {
			smileVideoList.push_back(slotSmileVideoSet[i]);
		}
	}
}

std::shared_ptr<std::list<std::string>> VideoPool::GetLastSmileVideoPath(int daysAgo)
{
	std::shared_ptr<std::list<std::string>> buffer = std::make_shared<std::list<std::string>>();

	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	boost::gregorian::date date = now.date() - boost::gregorian::days(daysAgo);

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(4) << static_cast<int>(date.year()) << "\\"
		<< std::setfill('0') << std::setw(2) << static_cast<int>(date.month()) << "\\"
		<< std::setfill('0') << std::setw(2) << static_cast<int>(date.day());

	boost::filesystem::path p(SMILE_VIDEO_PATH + "\\" + ss.str());

	int userCount = -1;
	if (boost::filesystem::is_directory(p)) {
		for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
			try {
				userCount = std::max(userCount, static_cast<int>(std::stoi(it->path().filename().string())));
			}
			catch (const std::exception &e) {
				std::cerr << "Folder index not a number: " << it->path().string() << " " << e.what() << std::endl;
			}
		}
	}

	for (int i = 0; i <= userCount; ++i) {
		boost::filesystem::path path(p);
		path.append(std::to_string(i)).append("0");
		if (boost::filesystem::is_directory(path)) {
			int fileCount = 0;
			for (boost::filesystem::directory_iterator it(path); it != boost::filesystem::directory_iterator(); ++it) {
				if (it->path().extension().string() == ".jpg")
					fileCount++;
			}
			if (fileCount == IMAGE_SEQUENCE_LENGTH)
				buffer->push_back(path.string());
		}
	}

	if (buffer->empty())
		return NULL;
	return buffer;
}

std::shared_ptr<cv::Mat> VideoPool::ReadImage(const std::string path)
{
	cv::Mat img = cv::imread(path);
	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
	cv::resize(img, *dst, cv::Size(img.cols / 2, img.rows / 2));

	return dst;
}



