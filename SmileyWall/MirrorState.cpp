/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>

using namespace std;
using namespace cv;

void MirrorState::processTime(Director *director, const int64 &currentTickCount)
{
	duration = director->getDuration(currentTickCount);
}

void MirrorState::processKeyEvent(Director *director, const int &key)
{
	switch(key) {
	case 't': case 'T':
		director->takePhoto(photo);
		break;
	case RIGHT_ARROW:
		director->setCanRecord(false);
		director->setAnimationState(new EndingState, true);
		break;
	default:
		break;
	}
}

void MirrorState::processMouseEvent(Director *director, const Point &mousePos)
{
}

void MirrorState::processOSC(Director *director, OSCListener *oscListener)
{
}

void MirrorState::processAnimation(Director *director)
{
	if (!director->getCanRecord()) {
		director->setCanRecord(true);
		return;
	}

	Picture& picture = *director->getPictureAt(27);
	picture.setContent(director->getRecordImage());

	for (int c = 0; c < numPicturesInPainting.width; c += 2) {
		for (int r = 0; r < numPicturesInPainting.height; r++) {
			int index = r * numPicturesInPainting.width + c;
			Picture& picture = *director->getPictureAt(index);
			picture.setContent(director->getImagesPool()->randomPhotos.at(index%numLookPeople).at(0));
		}
	}

	int photoIndex = 0;
	photoVec = director->getCaptureImageVec();
	if (photoVec.size() != 0) {
		for (int c = 1; c < numPicturesInPainting.width; c += 2) {
			for (int r = 0; r < numPicturesInPainting.height; r++) {
				int index = r * numPicturesInPainting.width + c;
				if (index == 27) 
					continue;

				Picture& picture = *director->getPictureAt(index);
				picture.setContent(photoVec[photoIndex]);
				photoIndex += 1;
				if (photoIndex >= photoVec.size()) {
					break;
				}
			}
		}
	}
	

	

////--- add by YuShan
//	Mat recordImage = director->getRecordImage();
//	recordImage *= 3;	// enhance record image
//	/*Mat hsvImage = Mat(image.rows, image.cols, CV_8UC3, image.pData);
//	cvtColor(recordImage, hsvImage, CV_RGB2HSV);
//	vector<Mat> mv;
//	split(hsvImage, mv);
//	equalizeHist(mv[2], mv[2]);
//	merge(mv, hsvImage);
//	cvtColor(hsvImage, recordImage, CV_HSV2RGB);*/
////---
//	Mat flipImage;
//	flip(recordImage, flipImage, 1);
//	
//	const int center = numPhotos / 2 + numPicturesInPainting.width / 2;
//	Picture& picture = *director->getPictureAt(center);
//	picture.setContent(flipImage);
//
//	if (!director->isAllAnimationEnd())
//		return;
//
//	ImagesPool *imagesPool = director->getImagesPool();
//	switch (showImageStep) {
//	case SHOW_CLEAN:	// clean all pictures
//		showImageStep = SHOW_LEFT;
//		for (int i = 0; i < numPhotos; ++i) {
//			Picture& picture = *director->getPictureAt(i);
//			picture.setContent(Mat(picture.size(), picture.type(), Scalar(255, 255, 255)));
//		}
//	case SHOW_LEFT:		// show image at the left of center
//		if (duration > 3.0f) {
//			showImageStep = SHOW_RIGHT;
//
//			if (isRandomed[0] == false) {
//				index[0] = rand() % numSmilePhotos;
//				isRandomed[0] = true;
//			}
//			
//			Picture& picture = *director->getPictureAt(center - 1);
//			picture.setContent(imagesPool->smilePhotos[index[0]]);
//		}
//		break;
//	case SHOW_RIGHT:	// show image at the right of center
//		if (duration > 4.0f) {
//			showImageStep = SHOW_LEFT_FLIP;
//
//			if (isRandomed[1] == false) {
//				do {
//					index[1] = rand() % numSmilePhotos;
//				} while (index[1] == index[0]);
//				isRandomed[1] = true;
//			}
//
//			Picture& picture = *director->getPictureAt(center + 1);
//			picture.setContent(imagesPool->smilePhotos[index[1]]);
//		}
//		break;
//	case SHOW_LEFT_FLIP:	// flip image at the left of center
//		if (duration > 5.0f) {
//			showImageStep = SHOW_WAIT;
//
//			if (isRandomed[2] == false) {
//				do {
//					index[2] = rand() % numSmilePhotos;
//				} while (index[2] == index[0] || index[2] == index[1]);
//				isRandomed[2] = true;
//			}
//
//			Picture& picture = *director->getPictureAt(center - 1);
//			picture.setAnimation(FlipAnimationEnum, imagesPool->smilePhotos[index[0]],
//				imagesPool->smilePhotos[index[2]]);
//
//			FlipAnimation *flipAnimation = static_cast<FlipAnimation *>(picture.getAnimation());
//			flipAnimation->setProperties(flips[0]);
//			flipAnimation->setCanReverse(false);
//			
//			picture.setEnableAnimationTime(director->getCurrentTime());
//		}
//		break;
//	case SHOW_WAIT:		// wait until 6 seconds
//		if (duration > 6.0f) {
//			director->setCanRecord(false);
//			director->setAnimationState(new TakePhotoState, true);
//		}
//		break;
//	default:
//		break;
//	}
}