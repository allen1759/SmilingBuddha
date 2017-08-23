/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SmileSaver.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <future>

#include <boost/date_time.hpp>

#include <opencv2/imgcodecs.hpp>

#include <thread>
#include <chrono>

SmileSaver * SmileSaver::instance = NULL;

SmileSaver::~SmileSaver()
{
	isRunning = false;
	saveThread.join();
}

SmileSaver * SmileSaver::GetInstance()
{
	if (instance == NULL)
		instance = new SmileSaver();

	return instance;
}

void SmileSaver::SaveImages(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	queueMutex.lock();
	imageSequenceQueue.push(images);
	queueMutex.unlock();
}

void SmileSaver::NextUser()
{
	// if last user has no smiling images, no need to save at next user index
	if (smileCount != 0) {
		++userCount;
		smileCount = 0;
	}
}

SmileSaver::SmileSaver()
{
	todayPath = boost::filesystem::path(GetDatePath());

	try {
		boost::filesystem::create_directories(todayPath);
	}
	catch (const boost::filesystem::filesystem_error & e) {
		std::cerr << e.what() << std::endl;
	}

	InitializeUserCount();
	smileCount = 0;

	isRunning = true;
	saveThread = std::thread(&SmileSaver::Saving, this);
}

std::string SmileSaver::GetDatePath() const
{
	// uncomplete
	// 1. use std time chrono
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(4) << static_cast<int>(now.date().year()) << "\\"
		<< std::setfill('0') << std::setw(2) << static_cast<int>(now.date().month()) << "\\"
		<< std::setfill('0') << std::setw(2) << static_cast<int>(now.date().day());
	
	return PATH + "\\" + ss.str();
}


std::string SmileSaver::GetTime() const
{
	// uncomplete
	// 1. use std time chrono
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

	return boost::posix_time::to_iso_string(now);
}

void SmileSaver::Saving()
{
	while (isRunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		//boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
		boost::filesystem::path p{ todayPath.string() + "\\" + std::to_string(userCount) + "\\" + std::to_string(smileCount) };
		
		queueMutex.lock();
		if (imageSequenceQueue.empty()) {
			queueMutex.unlock();
			continue;
		}
		std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images = imageSequenceQueue.front();
		imageSequenceQueue.pop();
		queueMutex.unlock();
		
		try {
			boost::filesystem::create_directories(p);

			std::string fileName = GetTime();
			for (int i = 0; i < images->size(); ++i) {
				std::string index = std::to_string(i);
				if (index.size() < INDEX_PADDING_LENGTH)
					index.insert(0, INDEX_PADDING_LENGTH - index.size(), '0');
				cv::imwrite(p.string() + "\\" + fileName + "_" + index + ".jpg", *images->at(i));
			}
		}
		catch (const boost::filesystem::filesystem_error & e) {
			std::cerr << e.what() << std::endl;
		}

		smileCount++;

		//boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
		//std::cout << "duration: " << (end - start).total_milliseconds() << std::endl;
	}
}

void SmileSaver::InitializeUserCount()
{
	// userCount = -1 indicate no folder.
	userCount = -1;
	for (boost::filesystem::directory_iterator it(todayPath); it != boost::filesystem::directory_iterator(); ++it) {
		if (!it->path().has_extension()) {
			int number = std::stoi(it->path().filename().string());
			userCount = userCount > number ? userCount : number;
		}
	}
	// Save images to next user index.
	++userCount;
}
