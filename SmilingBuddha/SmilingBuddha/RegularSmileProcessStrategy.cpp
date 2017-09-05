/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "RegularSmileProcessStrategy.h"

#include "Setting.h"

RegularSmileProcessStrategy::RegularSmileProcessStrategy(SmileObserver *observer)
	: SmileProcessStrategy(observer)
{
	this->imageSequenceLength = Setting::GetInstance()->GetImageSequenceLength();

	this->isRecord = false;
	this->imageBuffer = NULL;
}

RegularSmileProcessStrategy::~RegularSmileProcessStrategy()
{
}

void RegularSmileProcessStrategy::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
	if (!isRecord) {
		// if intensity > threshold, start recording smiles.
		if (intensity > SMILE_INTENSITY_THRESHOLD) {
			imageBuffer = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

			if (observer)
				observer->OnSmile();
			isRecord = true;
			imageBuffer->push_back(img);
		}
	}
	else {
		imageBuffer->push_back(img);

		// if buffer size reach target length, stop recording and pass images to SmileObserver.
		if (imageBuffer->size() == imageSequenceLength) {
			if (observer)
				observer->OnRecorded(imageBuffer);
			saver->SaveImages(imageBuffer);
			isRecord = false;
			imageBuffer = NULL;
		}
	}
}