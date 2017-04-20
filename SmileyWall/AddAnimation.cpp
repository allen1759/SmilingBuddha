#include "AddAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>

#define frameWidth 10

void AddAnimation::play(Picture &picture)
{
	cv::rectangle(picture, cv::Rect(0,0,picture.getFrame().width,frameWidth), CV_RGB(255,0,0), CV_FILLED);
	cv::rectangle(picture, cv::Rect(0,0,frameWidth,picture.getFrame().height), CV_RGB(255,0,0), CV_FILLED);
	cv::rectangle(picture, cv::Rect(picture.getFrame().width-frameWidth,0,picture.getFrame().width,picture.getFrame().height), CV_RGB(255,0,0), CV_FILLED);
	cv::rectangle(picture, cv::Rect(0,picture.getFrame().height-frameWidth,picture.getFrame().width,picture.getFrame().height), CV_RGB(255,0,0), CV_FILLED);
	//cv::circle(picture, cv::Point(55, 75), 20, CV_RGB(255,0,0), CV_FILLED);
	/*cv::Mat dst;
	double alpha = 0.7;
	addWeighted(picture, alpha, smileFace, 1-alpha, 0, dst);
	picture.setContent(dst);*/
}