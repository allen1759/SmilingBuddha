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

void EndingState::processTime(Director *director, const int64 &currentTickCount)
{
	duration = director->getDuration(currentTickCount);
}

void EndingState::processKeyEvent(Director *director, const int &key)
{

}

void EndingState::processMouseEvent(Director *director, const Point &mousePos)
{

}

void EndingState::processOSC(Director *director, OSCListener *oscListener)
{
	
}

void EndingState::processAnimation(Director *director)
{
	
	/*if (photoVec.empty()) {
		director->takePhoto(photo);
		photoVec.push_back(photo);
	}*/

	int center = numPhotos / 2 + numPicturesInPainting.width / 2;
	ImagesPool* imagesPool = director->getImagesPool();
	switch (endingStep)
	{
	case FADE_OUT_ALL:		// All pictures fade out
		{
			photoVec = director->getImagesPool()->currentUserPhotos;
			for (int i = 0; i < numPhotos; ++i) {
				Picture& picture = *director->getPictureAt(i);
				Mat content;
				picture.copyTo(content);
				picture.setAnimation(FadeAnimationEnum, content);

				FadeAnimation *fadeAnimation = static_cast<FadeAnimation *>(picture.getAnimation());
				fadeAnimation->setProperties(FADE_OUT);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}
			endingStep = USER_SMILE;
		}
		break;
	case USER_SMILE:		// Fade in user's correct answers and flip to name in pair sequentially
		{
			if (!director->isAllAnimationEnd())
				return;

			for (int i = 0; i < numPhotos; i++) {
				Picture& picture = *director->getPictureAt(i);
				//int rdm = rand() % photoVec.size();
				int rdm = i % photoVec.size();
				picture.setAnimation(FadeAnimationEnum, photoVec.at(rdm).at(0));

				int x = abs(i % numPicturesInPainting.width - center % numPicturesInPainting.width);
				int y = abs(i / numPicturesInPainting.width - center / numPicturesInPainting.width);
				int64 timeOffset = static_cast<int64>(getTickFrequency() * (log(static_cast<float>(x+y)) + abs(randNumGenerator.gaussian(3.0))));
				picture.setEnableAnimationTime(director->getCurrentTime() + timeOffset);
			}
			director->setStartTickCount();
			endingStep = USER_ANI;
		}
		break;
	case USER_ANI:
		{
			for (int i = 0; i < numPhotos; i++) {
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}
				isUserAniStart[i] = true;
				int rdm = i % photoVec.size();
				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setCanReverse(true);
				lookAnimation->setProperties(photoVec.at(rdm), videoTime);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}
			bool allStart = true;
			for(int i=0; i<44; i++) {
				if (isUserAniStart[i] == false) {
					allStart = false;
				}
			}
			if (allStart) {
				endingStep = EXPOSURE;
			}
		}
	case EXPOSURE:		// All pictures exposure
		{
			/*if (!director->isAllAnimationEnd())
				return;*/
			for (int i = 0; i < numPhotos; ++i) {
				Picture& picture = *director->getPictureAt(i);
				if (picture.getAnimation() && !picture.getAnimation()->animationEnded()) {
					continue;
				}
				isUserAniStart[i] = true;
				int rdm = i % photoVec.size();
				picture.setAnimation(LookAnimationEnum, picture);

				LookAnimation *lookAnimation = static_cast<LookAnimation *>(picture.getAnimation());
				lookAnimation->setCanReverse(true);
				lookAnimation->setProperties(photoVec.at(rdm), 8.0f);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}

			if (duration < 15.0f) 
				return;

			for (int i = 0; i < numPhotos; ++i) {
				Picture& picture = *director->getPictureAt(i);
				picture.getAnimation()->setEnd(true);
				picture.setAnimation(ExposureAnimationEnum, picture.clone());

				ExposureAnimation *exposureeAnimation = static_cast<ExposureAnimation *>(picture.getAnimation());
				exposureeAnimation->setProperties(pic2exp, 5, 0, 350);

				picture.setEnableAnimationTime(director->getCurrentTime());
			}
			endingStep = SHOW_TEXT;
		}
		break;
	case SHOW_TEXT:		// Show text
		{
			if (!director->isAllAnimationEnd())
				return;

			ImagesPool* imagesPool = director->getImagesPool();
			for (int i = 0; i < numPhotos; i++) {
				Picture& picture = *director->getPictureAt(i);
				picture.setAnimation(ExposureAnimationEnum, imagesPool->textPhotos[i]);

				ExposureAnimation *exposureeAnimation = static_cast<ExposureAnimation *>(picture.getAnimation());
				exposureeAnimation->setProperties(exp2pic, 1, 150, 0);
				picture.setEnableAnimationTime(director->getCurrentTime());				
			}
			/*for (int c = 0; c < numPicturesInPainting.width; c++) {
				for (int r = 0; r < numPicturesInPainting.height; r++) {
					Picture& picture = *director->getPictureAt(r * numPicturesInPainting.width + c);
					int pictureIndex = textOrder[c] * numPicturesInPainting.height + r;
					picture.setAnimation(ExposureAnimationEnum, imagesPool->textPhotos[pictureIndex]);

					ExposureAnimation *exposureeAnimation = static_cast<ExposureAnimation *>(picture.getAnimation());
					exposureeAnimation->setProperties(exp2pic, 5, 50);
					picture.setEnableAnimationTime(director->getCurrentTime());
				}
			}*/
			endingStep = END;
		}
		break;
	default:
		break;
	}
}