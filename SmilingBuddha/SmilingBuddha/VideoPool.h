/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_POOL_H
#define _VIDEO_POOL_H

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_map>

#include "ActorVideoSet.h"

class VideoPool
{
private:
	VideoPool();

public:
	~VideoPool();

	static VideoPool * GetInstance();

	std::shared_ptr<ActorVideoSet> GetActorVideoSet(int row, int col);

	void AddSmileVideo(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> newSmileVideo);

	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> GetSmileVideoList(int row, int col);

private:
	/**
	* Load slot smile video from file. The filesystem hierarchy show as followe:
	*		SLOT_PATH----00 (Sequence of images in folder)
	*				 |---05 (Sequence of images in folder)
	*				 |---08 (Sequence of images in folder)
	*				 |---32 (Sequence of images in folder)
	*
	* @param windowCount The number of total window.
	*/
	void LoadSlotSmileVideo(const int windowCount);

	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> LoadOneSlotVideo(const std::string path);

	/**
	* Load non-slot smile video from file. Save the newest smileVideo from SMILE_VIDEO_PATH.
	* The length of nonslotSmileVideoList should be (windowCount - slotSmileVideoList.size()).
	* ### Must called after LoadSlotSmileVideo() ###
	*
	* @param windowCount The number of total window.
	*/
	void LoadNonSlotSmileVideo(const int windowCount);

	/**
	* Get path list from days ago.
	*
	* @param daysAgo Value for how many days ago.
	* @return Path list at the day. 1st element is oldest, last element is newest.
	*/
	std::shared_ptr<std::list<std::string>> GetLastSmileVideoPath(int daysAgo);

	/**
	* Merge all smileVideo from slotSmileVideo and nonslotSmileVideo.
	*
	* @param windowCount The number of total window.
	*/
	void LoadAllSmileVideo(const int windowCount);

	// Read image from file and resize.
	std::shared_ptr<cv::Mat> ReadImage(const std::string path);


	static VideoPool *instance;

	const int ACTOR_COUNT = 38;

	// TODO: get from director
	const int IMAGE_SEQUENCE_LENGTH = 40;

	const std::string SMILE_VIDEO_PATH = "D:\\smileVideo";
	const std::string SLOT_PATH = "D:\\smileVideo\\slot";
	const std::string FILE_TYPE = ".jpg";

	std::vector<std::shared_ptr<ActorVideoSet>> actorVideoSets;

	// Slot videos.
	std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>> slotSmileVideoSet;
	// Non-slot videos. The 1st element is the newest; the last one is the oldest.
	std::list<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>> nonslotSmileVideoList;
	// All videos.
	std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>> smileVideoList;
};


#endif // !_VIDEO_POOL_H
