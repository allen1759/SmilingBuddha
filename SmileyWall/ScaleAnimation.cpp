#include "ScaleAnimation.h"
#include "Picture.h"
#include "Director.h"
#include <opencv2/opencv.hpp>
using namespace cv;

const int WINDOW_DISTANCE = 50;
const int WINDOW_DISTANCE_ROW_ADJUST = 18;

void ScaleAnimation::play(Picture &picture)
{
	handleVideoState();
	playVideo(picture);

	double currentTime = getTickCount() / getTickFrequency();
	startImage.copyTo(picture);
	double scaleFactor = scaleFrom + scaleSpeed * (scaleTo - scaleFrom) * (currentTime - startTime);
	double fadeFactor = fadeFrom + fadeSpeed * (fadeTo - fadeFrom) * (currentTime - startTime);

	if (fadeFactor < 0) {
		videoCanEnd = true;
		fadeFactor = 1;
	}

	if (scaleFactor != 0.0) {
		for(int i = 0; i < picture.getFrame().height; ++i) {
			for(int j = 0; j < picture.getFrame().width; ++j) {
				double sRow = i, sCol = j;

				translate(sRow, sCol, (thisRow - centerRow) * (WINDOW_DISTANCE + picture.getFrame().height) + WINDOW_DISTANCE_ROW_ADJUST, (thisCol - centerCol) * (WINDOW_DISTANCE + picture.getFrame().width));
				translate(sRow, sCol, -picture.getFrame().height/2, -picture.getFrame().width/2);
				scale(sRow, sCol, 1/scaleFactor, 1/scaleFactor);
				translate(sRow, sCol, sourceImage.rows/2, sourceImage.cols/2);

				// nearest neighbor interpolation
				int nnRow = int(sRow+0.5);
				int nnCol = int(sCol+0.5);
				if (nnRow < sourceImage.rows && nnRow >= 0 && nnCol < sourceImage.cols && nnCol >= 0) {
					if(sourceImage.at<Vec4b>(nnRow, nnCol)[3] != 0) {
						double alpha = max(0, double(sourceImage.at<Vec4b>(nnRow, nnCol)[3])/255.0 - fadeFactor);
						picture.at<Vec3b>(i, j)[0] = sourceImage.at<Vec4b>(nnRow, nnCol)[0] * alpha + 
							picture.at<Vec3b>(i, j)[0] * (1-alpha);
						picture.at<Vec3b>(i, j)[1] = sourceImage.at<Vec4b>(nnRow, nnCol)[1] * alpha +
							picture.at<Vec3b>(i, j)[1] * (1-alpha);
						picture.at<Vec3b>(i, j)[2] = sourceImage.at<Vec4b>(nnRow, nnCol)[2] * alpha +
							picture.at<Vec3b>(i, j)[2] * (1-alpha);
					}
				}
			}
		}
	}

	Mat originalColor = picture.clone();
	for(int i = 0; i < picture.getFrame().height; ++i) {
		for(int j = 0; j < picture.getFrame().width; ++j) {
			double offsetRow = (centerRow - thisRow) * (WINDOW_DISTANCE + picture.getFrame().height) - WINDOW_DISTANCE_ROW_ADJUST 
				+ picture.getFrame().height/2 - i;
			double offsetCol = (centerCol - thisCol) * (WINDOW_DISTANCE + picture.getFrame().width) + 
				picture.getFrame().width/2 - j;
			double distance = sqrt(offsetRow*offsetRow+offsetCol*offsetCol);
			double sinArg = (distance - (currentTime - startTime)*180.0)*0.09;
			double slope = 32.0 * sin(sinArg)/sinArg;
			int waveRow = min(max(static_cast<int>(i+offsetRow*slope/distance+0.5), 0), originalColor.rows - 1);
			int waveCol = min(max(static_cast<int>(j+offsetCol*slope/distance+0.5), 0), originalColor.cols - 1);
			picture.at<Vec3b>(i, j) = originalColor.at<Vec3b>(waveRow, waveCol);
		}
	}

}

void ScaleAnimation::translate(double &x, double &y, double dx, double dy)
{
	x += dx;
	y += dy;
}

void ScaleAnimation::scale(double &x, double &y, double dx, double dy)
{
	x *= dx;
	y *= dy;
}


void ScaleAnimation::handleVideoState()
{
	double currentTime = getTickCount() / getTickFrequency();
	switch(videoState) {
	case VIDEO_PLAY_FORWARD:
		if (currentTime - videoStartTime >= runTime) {
			videoState = VIDEO_PLAY_BACKWARD;
			videoStartTime = currentTime;
		}
		break;
	case VIDEO_PLAY_BACKWARD:
		if (currentTime - videoStartTime >= runTime) {
			videoState = VIDEO_PLAY_FORWARD;
			videoStartTime = currentTime;
			if(videoCanEnd)
				videoState = VIDEO_END;
		}
		break;
	case VIDEO_END:
		ended = true;
		break;
	default:
		break;
	}
}


void ScaleAnimation::playVideo(Picture &picture)
{
	double currentTime = getTickCount() / getTickFrequency();
	int currentPhotoIndex = (int)((currentTime - videoStartTime)/runTime*photoNum);
	if (currentPhotoIndex < 0 || currentPhotoIndex >= static_cast<int>(photoSet.size())) {
		return;
	}
	switch(videoState) {
	case VIDEO_PLAY_FORWARD:
		picture.setContent(photoSet.at(currentPhotoIndex));
		break;
	case VIDEO_PLAY_BACKWARD:
		currentPhotoIndex = photoNum - 1 - currentPhotoIndex;
		picture.setContent(photoSet.at(currentPhotoIndex));
		break;
	case VIDEO_END:
		break;
	default:
		break;
	}
}