/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SeeEachSmileProcessStrategy.h"

#include <algorithm>

#include "Setting.h"

SeeEachSmileProcessStrategy::SeeEachSmileProcessStrategy(SmileObserver * observer, float waitTime)
	: SmileProcessStrategy(observer)
{
	this->imageSequenceLength = Setting::GetInstance()->GetImageSequenceLength();

	this->hasAnySmile = false;
	this->waitTime = std::chrono::milliseconds(static_cast<int>(waitTime * 1000));
	this->startTime = std::chrono::high_resolution_clock::now();

	this->isRecord = false;
	this->imageBuffer = NULL;
}

SeeEachSmileProcessStrategy::~SeeEachSmileProcessStrategy()
{
}

void SeeEachSmileProcessStrategy::ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity)
{
	if (!isRecord) {
		// if intensity > threshold, start recording smiles.
		if (intensity > SMILE_INTENSITY_THRESHOLD) {
			imageBuffer = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();

			if (observer)
				observer->OnSmile();
			isRecord = true;
			imageBuffer->push_back(img);

			hasAnySmile = true;
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

	allImageBuffer.push_back(img);
	allIntensityBuffer.push_back(intensity);

	if (!hasAnySmile) {
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime) > waitTime)
			SelectBestSmile();
	}
}

void SeeEachSmileProcessStrategy::SelectBestSmile()
{
	if (allIntensityBuffer.size() < imageSequenceLength)
		return;

	if (observer)
		observer->OnSmile();

	std::vector<double>::iterator maxIt = std::max_element(allIntensityBuffer.begin(), allIntensityBuffer.end());
	int maxIndex = maxIt - allIntensityBuffer.begin();
	int firstIndex = maxIndex - (imageSequenceLength >> 1);
	int lastIndex = firstIndex + imageSequenceLength;

	if (firstIndex < 0) {
		firstIndex = 0;
		lastIndex = imageSequenceLength;
	}
	else if (lastIndex > allIntensityBuffer.size()) {
		lastIndex = allIntensityBuffer.size();
		firstIndex = lastIndex - imageSequenceLength;
	}

	imageBuffer = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
	for (int i = firstIndex; i < lastIndex; ++i)
		imageBuffer->push_back(allImageBuffer.at(i));
	if (observer)
		observer->OnRecorded(imageBuffer);
	saver->SaveImages(imageBuffer);
	imageBuffer = NULL;

	hasAnySmile = true;
	allImageBuffer.clear();
	allIntensityBuffer.clear();
}