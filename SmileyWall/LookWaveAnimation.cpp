#include "LookWaveAnimation.h"
#include "Picture.h"
#include "Director.h"
#include <opencv2/opencv.hpp>
using namespace cv;

const float thresholdNumNonZero = 500;

void LookWaveAnimation::play(Picture &picture)
{
	handleVideoState();
	playVideo(picture);

	damping = (speedUp)? 2 : damping;
	display(picture);
	updateWaveEnergy();
}


void LookWaveAnimation::initActiveWaveEnergy()
{
	// set center, center = center of the image
	Point center(activeWaveEnergy.cols >> 1, activeWaveEnergy.rows >> 1);
	for (int i = -radius; i <= radius; ++i) {
		// check boundary
		if ((center.x + i >= 0) && (center.x + i < activeWaveEnergy.cols - 1)) {
			for (int j = -radius; j <= radius; ++j) {
				double dist = sqrt(static_cast<double>(i*i + j*j));
				if (dist > radius)
					continue;

				// wave function
				if ((center.y + j >= 0) && (center.y + j < activeWaveEnergy.rows - 1)) {
					activeWaveEnergy.at<short>(center.y + j, center.x + i) = 
						static_cast<short>(cos(dist * M_PI / radius) * initEnergy);
				}
			}
		}
	}
}

void LookWaveAnimation::updateWaveEnergy()
{	
	for (int i = 1; i < newWaveEnergy.rows - 1; ++i) {
		for (int j = 1; j < newWaveEnergy.cols - 1; ++j) {
			// calculate pixel value (blur + wave)
			newWaveEnergy.at<short>(i, j) = static_cast<short>(
				((activeWaveEnergy.at<short>(i - 1, j - 1) +
				activeWaveEnergy.at<short>(i - 1, j) +
				activeWaveEnergy.at<short>(i - 1, j + 1) +
				activeWaveEnergy.at<short>(i, j - 1) +
				activeWaveEnergy.at<short>(i, j + 1) +
				activeWaveEnergy.at<short>(i + 1, j - 1) +
				activeWaveEnergy.at<short>(i + 1, j) +
				activeWaveEnergy.at<short>(i + 1, j + 1)) >> 2) - newWaveEnergy.at<short>(i, j));

			// wave decay
			newWaveEnergy.at<short>(i, j) -= (newWaveEnergy.at<short>(i, j) >> damping);
		}
	}

	cv::swap(activeWaveEnergy, newWaveEnergy);
}
void LookWaveAnimation::display(Picture &picture)
{
	int numNonZero = countNonZero(activeWaveEnergy);
	if (numNonZero == 0)
		videoCanEnd = true;

	if (!isAlmostWaveEnergyIsZero &&
		abs(numNonZero - (activeWaveEnergy.rows) * (activeWaveEnergy.cols)) < 
		thresholdNumNonZero) {
			isAlmostWaveEnergyIsZero = true;
	}

	// calculate boundary
	sourceImage = picture.clone();
	for (int i = 0; i < sourceImage.rows; ++i) {
		for (int j = 0; j < sourceImage.cols; ++j) {
			Point wave(min(max(j >> scale, 1), activeWaveEnergy.cols - 2), 
				min(max(i >> scale, 1), activeWaveEnergy.rows - 2));

			Point offset((activeWaveEnergy.at<short>(wave.y, wave.x - 1) - activeWaveEnergy.at<short>(wave.y, wave.x + 1)) >> 5,
				(activeWaveEnergy.at<short>(wave.y - 1, wave.x) - activeWaveEnergy.at<short>(wave.y + 1, wave.x)) >> 5);

			if (offset.x != 0 || offset.y != 0) {
				Point newPoint(min(max(j + offset.x, 0), sourceImage.cols - 1),
					min(max(i + offset.y, 0), sourceImage.rows - 1));

				picture.at<Vec3b>(i, j) = sourceImage.at<Vec3b>(newPoint.y, newPoint.x);
			}
			else if (isAlmostWaveEnergyIsZero)
				picture.at<Vec3b>(i, j) = sourceImage.at<Vec3b>(i, j);
		}
	}
}

void LookWaveAnimation::handleVideoState()
{
	double currentTime = getTickCount() / getTickFrequency();
	switch(lookWaveState) {
	case LOOKWAVE_PLAY_FORWARD:
		if (currentTime - videoStartTime >= runTime) {
			lookWaveState = LOOKWAVE_PLAY_BACKWARD;
			videoStartTime = currentTime;
		}
		break;
	case LOOKWAVE_PLAY_BACKWARD:
		if (currentTime - videoStartTime >= runTime) {
			lookWaveState = LOOKWAVE_PLAY_FORWARD;
			videoStartTime = currentTime;
			if(videoCanEnd)
				lookWaveState = LOOKWAVE_END;
		}
		break;
	case LOOKWAVE_END:
		ended = true;
		break;
	default:
		break;
	}
}


void LookWaveAnimation::playVideo(Picture &picture)
{
	double currentTime = getTickCount() / getTickFrequency();
	int currentPhotoIndex = (int)((currentTime - videoStartTime)/runTime*photoNum);
	if (currentPhotoIndex < 0 || currentPhotoIndex >= static_cast<int>(photoSet.size())) {
		return;
	}
	switch(lookWaveState) {
	case LOOKWAVE_PLAY_FORWARD:
		picture.setContent(photoSet.at(currentPhotoIndex));
		break;
	case LOOKWAVE_PLAY_BACKWARD:
		currentPhotoIndex = photoNum - 1 - currentPhotoIndex;
		picture.setContent(photoSet.at(currentPhotoIndex));
		break;
	case LOOKWAVE_END:
		break;
	default:
		break;
	}
}