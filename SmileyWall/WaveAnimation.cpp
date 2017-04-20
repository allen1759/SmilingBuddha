#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <omp.h>
#include "WaveAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;

const float thresholdNumNonZero = 500;

void WaveAnimation::play(Picture &picture)
{
	damping = (speedUp)? 2 : damping;
	display(picture);
	updateWaveEnergy();
}

void WaveAnimation::initActiveWaveEnergy()
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

void WaveAnimation::updateWaveEnergy()
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
void WaveAnimation::display(Picture &picture)
{
	int numNonZero = countNonZero(activeWaveEnergy);
	if (numNonZero == 0) {
		picture.setContent(sourceImage);
		if (canReverse) {
			isAlmostWaveEnergyIsZero = false;
			canReverse = false;
			sourceImage = startImage;
			initActiveWaveEnergy();
		}
		else {
			ended = true;
			return;
		}
	}

	if (!isAlmostWaveEnergyIsZero &&
		abs(numNonZero - (activeWaveEnergy.rows) * (activeWaveEnergy.cols)) < 
			thresholdNumNonZero) {

		isAlmostWaveEnergyIsZero = true;
	}

	// calculate boundary
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