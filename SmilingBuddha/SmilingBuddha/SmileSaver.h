/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_SAVER_H
#define _SMILE_SAVER_H

#include <string>
#include <vector>
#include <memory>

#include <boost/filesystem.hpp>

#include <opencv2/core.hpp>

class SmileSaver
{
public:
	static SmileSaver * GetInstance();
	/**
	* Save images.
	*
	* @param images	Vector of cv::Mat images to be saved.
	*
	*/
	void BeginSaveImages(const std::shared_ptr<const std::vector<cv::Mat>> images);

	/**
	* Call this function to save image to next user. It should be called if any
	* user detection triggered.
	*/
	void NextUser();

private:
	SmileSaver();

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
	* @param images	Vector of cv::Mat images to be saved.
	* @param userCount Index for user folder.
	* @param smileCount Index for smile folder.
	*
	*/
	void SaveImages(const std::shared_ptr<const std::vector<cv::Mat>> images, int userCount, int smileCount) const;

	void InitializeUserCount();


	static SmileSaver *instance;

	// The path to store all images.
	const std::string PATH = "D:\\smileVideo";
	boost::filesystem::path todayPath;
	
	// Counting the number of user in the current day folder.
	int userCount;
	// Counting the number of smiling in the current user folder.
	int smileCount;
};

#endif // !_SMILE_SAVER_H
