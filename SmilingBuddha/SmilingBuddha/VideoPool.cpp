/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoPool.h"

#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Setting.h"

VideoPool * VideoPool::instance = NULL;

VideoPool::VideoPool() : COL_COUNT(Setting::GetInstance()->GetCol()),
					     ROW_COUNT(Setting::GetInstance()->GetRow()),
						 IMAGE_SEQUENCE_LENGTH(Setting::GetInstance()->GetImageSequenceLength()),
						 IMAGE_WIDTH(Setting::GetInstance()->GetImageWidth()),
						 IMAGE_HEIGHT(Setting::GetInstance()->GetImageHeight())
{
	CalculateSmileVideoOrder(COL_COUNT * ROW_COUNT);
	LoadSlotSmileVideo(COL_COUNT * ROW_COUNT);
	LoadNonSlotSmileVideo(COL_COUNT * ROW_COUNT);

	LoadAllSmileVideo(COL_COUNT * ROW_COUNT);

	for (int i = 0; i < ROW_COUNT; ++i) {
		for (int j = 0; j < COL_COUNT; ++j) {
			// ========== for debug ==========
			if (i != 0 || j != 0) {
				actorVideoSets.push_back(actorVideoSets[0]);
				continue;
			}
			// ========== for debug ==========

			int index = Setting::GetInstance()->GetActorIndex(i, j);
			actorVideoSets.push_back(std::make_shared<ActorVideoSet>(index));
		}
	}

	LoadBuddhasImages();
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
	return actorVideoSets[row * COL_COUNT + col];
}

void VideoPool::AddSmileVideo(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> newSmileVideo)
{
	nonslotSmileVideoList.pop_back();
	nonslotSmileVideoList.push_front(newSmileVideo);

	LoadAllSmileVideo(COL_COUNT * ROW_COUNT);
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetSmileVideoList(int row, int col)
{
	return smileVideoList[row * COL_COUNT + col];
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetBuddhaAnimationVideo()
{
	return buddhaAnimationVideo;
}

std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> VideoPool::GetNextBuddhaVideo()
{
	buddhaImageListIndex = (buddhaImageListIndex + 1) % buddhaVideoList.size();
	return buddhaVideoList.at(buddhaImageListIndex);
}

void VideoPool::CalculateSmileVideoOrder(const int windowCount)
{
	// Row and column of center grid.
	std::pair<int, int> position;
	position.first = Setting::GetInstance()->GetCenterRow();
	position.second = Setting::GetInstance()->GetCenterCol();

	smileVideoOrder.push_back(position);
	int level = 1;
	while (smileVideoOrder.size() < windowCount) {
		// Move up one position.
		position.first--;
		if (position.first >= 0 && position.first < ROW_COUNT &&
			position.second >= 0 && position.second < COL_COUNT) {
			smileVideoOrder.push_back(position);
		};

		// Width of current level.
		int width = level * 2;

		// Add top line to order.
		for (int i = 0; i < width - 1; ++i) {
			position.second++;
			if (position.first >= 0 && position.first < ROW_COUNT &&
				position.second >= 0 && position.second < COL_COUNT) {
				smileVideoOrder.push_back(position);
			}
		}
		// Add right line to order.
		for (int i = 0; i < width; ++i) {
			position.first++;
			if (position.first >= 0 && position.first < ROW_COUNT &&
				position.second >= 0 && position.second < COL_COUNT) {
				smileVideoOrder.push_back(position);
			}
		}
		// Add bottom line to order.
		for (int i = 0; i < width; ++i) {
			position.second--;
			if (position.first >= 0 && position.first < ROW_COUNT &&
				position.second >= 0 && position.second < COL_COUNT) {
				smileVideoOrder.push_back(position);
			}
		}
		// Add left line to order.
		for (int i = 0; i < width; ++i) {
			position.first--;
			if (position.first >= 0 && position.first < ROW_COUNT &&
				position.second >= 0 && position.second < COL_COUNT) {
				smileVideoOrder.push_back(position);
			}
		}

		// Move to next level
		level++;
	}
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
				if (slotIndex < windowCount) {
					std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imgs = LoadOneSlotVideo(it->path().string());
					if (imgs != NULL)
						slotSmileVideoSet[slotIndex] = imgs;
				}
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
		if (it->path().extension().string() == SLOT_FILE_TYPE)
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
			if (it->path().extension().string() == SLOT_FILE_TYPE)
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
				if (it->path().extension().string() == SLOT_FILE_TYPE)
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
	smileVideoList.clear();
	smileVideoList.resize(windowCount);
	std::list<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>::iterator it = nonslotSmileVideoList.begin();

	for (int i = 0; i < windowCount; ++i) {
		// If current index has no slotSmileVideo
		int currentIndex = smileVideoOrder[i].first * COL_COUNT + smileVideoOrder[i].second;

		if (slotSmileVideoSet[currentIndex] == NULL) {
			smileVideoList.at(currentIndex) = (*it);
			it++;
		}
		else
			smileVideoList.at(currentIndex) = slotSmileVideoSet[currentIndex];
	}
}

void VideoPool::LoadBuddhasImages()
{
	buddhaAnimationVideo = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
	buddhaAnimationVideo->push_back(ReadImage("resources\\buddha_animation_image" + FILE_TYPE));

	boost::filesystem::path p("resources\\buddhas\\");

	// Iterate all files in path.
	for (boost::filesystem::directory_iterator it(p); it != boost::filesystem::directory_iterator(); ++it) {
		// Check the target is match to the FILE_TYPE.
		if (it->path().extension().string() == FILE_TYPE) {
			std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageSequence = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
			imageSequence->push_back(ReadImage(it->path().string()));
			buddhaVideoList.push_back(imageSequence);
		}
	}
	if (!buddhaVideoList.empty())
		buddhaImageListIndex = rand() % buddhaVideoList.size();
	else
		std::cerr << "Buddha images not found. " << std::endl;
}

std::shared_ptr<cv::Mat> VideoPool::ReadImage(const std::string path)
{
	cv::Mat cacheMat = cv::imread(path);
	std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
	cv::resize(cacheMat, *dst, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	return dst;
}

