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


SmileSaver * SmileSaver::instance = NULL;

SmileSaver * SmileSaver::GetInstance()
{
	if (instance == NULL)
		instance = new SmileSaver();

	return instance;
}

void SmileSaver::BeginSaveImages(const std::shared_ptr<const std::vector<cv::Mat>> images)
{
	std::async(std::launch::async, &SmileSaver::SaveImages, this, images, userCount, smileCount);
	++smileCount;
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

void SmileSaver::SaveImages(const std::shared_ptr<const std::vector<cv::Mat>> images, int userCount, int smileCount) const
{
	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
	boost::filesystem::path p{ todayPath.string() + "\\" + std::to_string(userCount) + "\\" + std::to_string(smileCount) };
	try {
		boost::filesystem::create_directories(p);

		std::string fileName = GetTime();
		for (int i = 0; i < images->size(); ++i)
			cv::imwrite(p.string() + "\\" + GetTime() + "_" + std::to_string(i) + ".jpg", images->at(i));
	}
	catch (const boost::filesystem::filesystem_error & e) {
		std::cerr << e.what() << std::endl;
	}
	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
	std::cout << "duration: " << (end - start).total_milliseconds() << std::endl;
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
