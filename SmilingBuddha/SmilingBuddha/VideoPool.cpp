/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoPool.h"

#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

#include "Setting.h"

VideoPool * VideoPool::instance = NULL;

VideoPool::VideoPool() : WINDOW_COL_COUNT(Setting::GetInstance()->GetCol()),
					     WINDOW_ROW_COUNT(Setting::GetInstance()->GetRow()),
						 IMAGE_SEQUENCE_LENGTH(Setting::GetInstance()->GetImageSequenceLength())
{
	LoadSlotSmileVideo(WINDOW_COL_COUNT * WINDOW_ROW_COUNT);
	LoadNonSlotSmileVideo(WINDOW_COL_COUNT * WINDOW_ROW_COUNT);

	LoadAllSmileVideo(WINDOW_COL_COUNT * WINDOW_ROW_COUNT);

	for (int i = 0; i < WINDOW_ROW_COUNT; ++i) {
		for (int j = 0; j < WINDOW_COL_COUNT; ++j) {
			int index = Setting::GetInstance()->GetActorIndex(i, j);
			actorVideoSets.push_back(std::make_shared<ActorVideoSet>(index));
		}
	}
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

std::shared_ptr<ActorVideoSet> VideoPool::GetActorVideoSet(int row, int col)
{
	return actorVideoSets[row * WINDOW_COL_COUNT + col];
}

void VideoPool::AddSmileVideo(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> newSmileVideo)
{
	nonslotSmileVideoList.pop_back();
	nonslotSmileVideoList.push_front(newSmileVideo);

	LoadAllSmileVideo(WINDOW_COL_COUNT * WINDOW_ROW_COUNT);
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetSmileVideoList(int row, int col)
{
	return smileVideoList[row * WINDOW_COL_COUNT + col];
}

void VideoPool::LoadSlotSmileVideo(const int windowCount)
{
	boost::filesystem::path p(SLOT_PATH);
	// Iterate all folders in SLOT_PATH.
	for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
		try {
			// Check the target is folder not file.
			if (!it->path().has_extension()) {
				int slotIndex = std::stoi(it->path().filename().string());
				// Check the number(folder name) exceed total windowCount or not.
				if (slotIndex < windowCount)
					slotSmileVideoSet[slotIndex] = LoadOneSlotVideo(it->path().string());
			}
		}
		// If string to int failed, catch exeption.
		catch (const std::exception &e) {
			std::cerr << "Load Slot SmileVideo Error: " << e.what() << std::endl;
		}
	}
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::LoadOneSlotVideo(const std::string path)
{
	boost::filesystem::path p(path);
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

	// Iterate all files in path.
	for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
		// Check the target is match to the FILE_TYPE.
		if (it->path().extension().string() == FILE_TYPE)
			imgs->push_back(ReadImage(it->path().string()));
	}

	if (imgs->size() != IMAGE_SEQUENCE_LENGTH)
		return NULL;
	return imgs;
}

void VideoPool::LoadNonSlotSmileVideo(const int windowCount)
{
	// nonslot length + slot length should equal to windowCount.
	int nonSlotSmileLength = windowCount - slotSmileVideoSet.size();
	// daysAgo = 0 means start searching from today.
	int daysAgo = 0;
	std::shared_ptr<std::list<std::string>> smileVideoPathList = NULL;

	while (nonslotSmileVideoList.size() < nonSlotSmileLength) {
		// Get smileVideoPath from last day. If the number not enough, get path from one more day ago.
		while (smileVideoPathList == NULL || smileVideoPathList->empty()) {
			smileVideoPathList = GetLastSmileVideoPath(daysAgo);
			daysAgo++;
		}

		// Get the newest path from list (last element in list).
		std::string path = smileVideoPathList->back();
		smileVideoPathList->pop_back();

		std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
		boost::filesystem::path p(path);
		// Iterate all files in path.
		for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
			// Check the target is match to the FILE_TYPE.
			if (it->path().extension().string() == FILE_TYPE)
				imgs->push_back(ReadImage(it->path().string()));
		}

		if (imgs->size() == IMAGE_SEQUENCE_LENGTH)
			nonslotSmileVideoList.push_back(imgs);
	}
}

std::shared_ptr<std::list<std::string>> VideoPool::GetLastSmileVideoPath(int daysAgo)
{
	std::shared_ptr<std::list<std::string>> buffer = std::make_shared<std::list<std::string>>();

	// Find the date couple days ago. (value = daysAgo)
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	boost::gregorian::date date = now.date() - boost::gregorian::days(daysAgo);

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(4) << static_cast<int>(date.year()) << "\\"
		<< std::setfill('0') << std::setw(2) << static_cast<int>(date.month()) << "\\"
		<< std::setfill('0') << std::setw(2) << static_cast<int>(date.day());

	boost::filesystem::path p(SMILE_VIDEO_PATH + "\\" + ss.str());

	// find the max user count in path.
	int userCount = -1;
	if (boost::filesystem::is_directory(p)) {
		// Iterate all folders(user count in a day) in path.
		for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
			try {
				userCount = std::max(userCount, static_cast<int>(std::stoi(it->path().filename().string())));
			}
			// If string to int failed, catch exeption.
			catch (const std::exception &e) {
				std::cerr << "Folder index not a number: " << it->path().string() << " " << e.what() << std::endl;
			}
		}
	}

	for (int i = 0; i <= userCount; ++i) {
		boost::filesystem::path path(p);
		// Only check the first folder (folder name: "0").
		path.append(std::to_string(i)).append("0");
		if (boost::filesystem::is_directory(path)) {
			int fileCount = 0;
			// Iterate all files in path.
			for (boost::filesystem::directory_iterator it(path); it != boost::filesystem::directory_iterator(); ++it) {
				// Check the target is match to the FILE_TYPE.
				if (it->path().extension().string() == FILE_TYPE)
					fileCount++;
			}
			// Check the length equal to IMGAGE_SEQUENCE_LENGTH.
			if (fileCount == IMAGE_SEQUENCE_LENGTH)
				buffer->push_back(path.string());
		}
	}

	if (buffer->empty())
		return NULL;
	return buffer;
}

void VideoPool::LoadAllSmileVideo(const int windowCount)
{
	std::list<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>::iterator it = nonslotSmileVideoList.begin();

	for (int i = 0; i < windowCount; ++i) {
		// If current index has no slotSmileVideo
		if (slotSmileVideoSet[i] == NULL) {
			smileVideoList.push_back(*it);
			it++;
		}
		else
			smileVideoList.push_back(slotSmileVideoSet[i]);
	}
}

std::shared_ptr<cv::Mat> VideoPool::ReadImage(const std::string path)
{
	cv::Mat img = cv::imread(path);
	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
	cv::resize(img, *dst, cv::Size(img.cols >> 1, img.rows >> 1));

	return dst;
}

