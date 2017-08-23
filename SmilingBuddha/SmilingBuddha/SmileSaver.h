/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_SAVER_H
#define _SMILE_SAVER_H

#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

#include <boost/filesystem.hpp>

#include <opencv2/core.hpp>

class SmileSaver
{
private:
	SmileSaver();

public:
	~SmileSaver();

	static SmileSaver * GetInstance();
	/**
	* Save images.
	*
	* @param images	Vector of cv::Mat images to be saved.
	*
	*/

	void SaveImages(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images);

	/**
	* Call this function to save image to next user. It should be called if any
	* user detection triggered.
	*/
	void NextUser();

private:

	/**
	* Get path of local date. The file system hierarchy show as follow:
	*               Year----Month1----Day1----User1
	*	                |         |       |---User2
	*	                |         |       |---User3
	*	                |         |           .....
	*	                |         |---Day2----User1
	*	                |         |       |---User2
	*	                |         |           .....
	*	                |         |---Day3----User1
	*	                |         |           .....
	*                   |---Month2----Day1----User1
	*	                |                     .....
	*
	* @return The path format is $PATH$\YYYY\MM\DD.
	*
	*/
	std::string GetDatePath() const;

	/**
	* Get local time.
	*
	* @return string of local time with ISO 8601  format. (e.g. YYYYMMDDTHHMMSS)
	*/
	std::string GetTime() const;

	/**
	* Save images.
	*
	*/
	void Saving();

	void InitializeUserCount();


	static SmileSaver *instance;

	// The path to store all images.
	const std::string PATH = "D:\\smileVideo";
	const int INDEX_PADDING_LENGTH = 2;
	boost::filesystem::path todayPath;
	
	// Counting the number of user in the current day folder.
	int userCount;
	// Counting the number of smiling in the current user folder.
	int smileCount;

	std::mutex queueMutex;
	std::queue <std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>> imageSequenceQueue;
	bool isRunning;
	std::thread saveThread;
};

#endif // !_SMILE_SAVER_H
