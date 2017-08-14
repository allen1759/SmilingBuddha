/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "RegularSmileProcessStrategy.h"

RegularSmileProcessStrategy::RegularSmileProcessStrategy(SmileObserver * observer, int imageSequenceLength)
{
	this->observer = observer;
}

RegularSmileProcessStrategy::~RegularSmileProcessStrategy()
{
}

void RegularSmileProcessStrategy::InitializeProcess()
{
}

void RegularSmileProcessStrategy::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
	if (!isRecord) {
		// if intensity > threshold, start recording smiles.
		if (intensity > SMILE_INTENSITY_THRESHOLD) {
			imageBuffer = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
			//intensityBuffer.clear();

			observer->OnSmile();
			isRecord = true;
			imageBuffer->push_back(img);
			//intensityBuffer.push_back(intensity);
		}
	}
	else {
		imageBuffer->push_back(img);
		//intensityBuffer.push_back(intensity);

		// if buffer size reach target length, stop recording and pass images to SmileObserver.
		if (imageBuffer->size() == IMAGE_SEQUENCE_LENGTH) {
			observer->OnRecorded(imageBuffer);
			isRecord = false;
			imageBuffer = NULL;
			//intensityBuffer.clear();
		}
	}
}