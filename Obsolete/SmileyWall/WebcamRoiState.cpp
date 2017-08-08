/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void WebcamRoiState::processTime(Director *director, const int64 &currentTickCount)
{

}

void WebcamRoiState::processKeyEvent(Director *director, const int &key)
{
	Rect roi = director->getWebcamRoi();
	switch(key) {
	case LEFT_ARROW:
		if (roi.x - 2 >= 0) {
			roi.x -= 2;
			director->setWebcamRoi(roi);
		}
		break;
	case UP_ARROW:
		if (roi.y - 2 >= 0) {
			roi.y -= 2;
			director->setWebcamRoi(roi);
		}
		break;
	case RIGHT_ARROW:
		if (roi.x + 2 + roi.width <= webcamImageSize.width) {
			roi.x += 2;
			director->setWebcamRoi(roi);
		}
		break;
	case DOWN_ARROW:
		if (roi.y + 2 + roi.height <= webcamImageSize.height) {
			roi.y += 2;
			director->setWebcamRoi(roi);
		}
		break;
	case '[':
		{
			Rect newROI(roi.x + 3, roi.y + 4, roi.width - 6, roi.height - 8);
			if (newROI.width >= 120 && newROI.height >= 160 && 
				newROI.x + newROI.width <= webcamImageSize.width &&
				newROI.y + newROI.height <= webcamImageSize.height) {
					director->setWebcamRoi(newROI);
			}
		}
		break;
	case ']':
		{
			Rect newROI(roi.x - 3, roi.y - 4, roi.width + 6, roi.height + 8);
			if (newROI.x + newROI.width <= webcamImageSize.width && 
				newROI.y + newROI.height <= webcamImageSize.height && 
				newROI.x >= 0 && newROI.y >= 0) {
					director->setWebcamRoi(newROI);
			}
		}
		break;
	case ENTER:
		{
			ofstream ofs("webcamROI.txt");
			if (ofs.good()) {
				ofs << roi.x << " " << roi.y << " " << 
						roi.width << " " << roi.height << endl;
			}
			else
				cerr << "Writing a setting file fails!" << endl;
			ofs.close();

			director->setAnimationState(new InitState);
		}
		break;
	default:
		break;
	}
}

void WebcamRoiState::processMouseEvent(Director *director, const Point &mousePos)
{
}

void WebcamRoiState::processOSC(Director *director, OSCListener *oscListener)
{
}

void WebcamRoiState::processAnimation(Director *director)
{
	if (!director->getCanRecord()) {
		director->setCanRecord(true);
		return;
	}
	
	Mat recordImage = director->getRecordImage();
	//recordImage *= 4.5;	// enhance record image
	Mat flipImage;
	flip(recordImage, flipImage, 1);
	
	for (int i = 0; i < numPhotos; ++i) {
		Picture& picture = *director->getPictureAt(i);
		if (i == 27) // 27: center
			picture.setContent(flipImage);
		else
			picture.setContent(Mat(picture.size(), picture.getType(), Scalar(255, 255, 255)));
	}
}