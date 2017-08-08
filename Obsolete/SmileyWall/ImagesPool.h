/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef IMAGES_POOL_H
#define IMAGES_POOL_H

#include <opencv2/opencv.hpp>

/*!
 * \brief
 * A pool contains all used images.

   If you need to read images from files, please cache these data in this class.
 * 
 */

struct SlotUser {
	std::vector<cv::Mat> photos;
	int index;
};

class ImagesPool
{
public:
	std::vector<cv::Mat> starPhotos; //!< all star photos
	std::vector<cv::Mat> smilePhotos; //!< all smile photos
	std::vector<cv::Mat> textPhotos; //!< all text photos
	std::vector<cv::Mat> similarPhotos; //!< all similar photos

	std::vector<cv::Mat> leftTopPhotos;
	std::vector<cv::Mat> leftBottomPhotos;
	std::vector<cv::Mat> rightTopPhotos;
	std::vector<cv::Mat> rightBottomPhotos;
	std::vector<cv::Mat> centerPhotos;

	std::vector<cv::Mat> smileFacePhotos;
	//person->direction(directory)->photos
	std::vector<std::vector<std::vector<cv::Mat>>> allLookPhotos;
	/*std::vector<std::vector<cv::Mat>> centerLookPhotosVec;
	std::vector<std::vector<cv::Mat>> upLookPhotosVec;
	std::vector<std::vector<cv::Mat>> downLookPhotosVec;
	std::vector<std::vector<cv::Mat>> leftLookPhotosVec;
	std::vector<std::vector<cv::Mat>> rightLookPhotosVec;
	std::vector<std::vector<cv::Mat>> leftUpLookPhotosVec;
	std::vector<std::vector<cv::Mat>> leftDownLookPhotosVec;
	std::vector<std::vector<cv::Mat>> rightUpLookPhotosVec;
	std::vector<std::vector<cv::Mat>> rightDwonLookPhotosVec;*/

	std::vector<std::vector<cv::Mat> > randomPhotos;
	std::vector<std::vector<cv::Mat> > userPhotos;
	std::vector<std::vector<cv::Mat> > crazyUserPhotos;
	std::vector<std::vector<cv::Mat> > currentUserPhotos;
	std::vector<cv::Mat> buddhaPhotos;

	std::vector<std::vector<cv::Mat>> smileVideoPhotos;
	std::vector<std::vector<cv::Mat>> smileUserPhotos;
	std::vector<SlotUser> slotUserPhotos;
};

#endif // IMAGES_POOL_H