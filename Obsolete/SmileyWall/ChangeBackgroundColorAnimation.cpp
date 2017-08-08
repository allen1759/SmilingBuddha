#include "ChangeBackgroundColorAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>

void ChangeBackgroundColorAnimation::play(Picture &picture)
{
	Mat mask;
	Mat tempPicture = picture.clone();
	Mat tmpimg = picture.clone();
	Mat maskclear(picture.size(), CV_8U, cv::Scalar(255));
	const int framenum = 35;
	Mat maskblur[framenum], maskdilate[framenum+1];
	cvtColor(tmpimg, tmpimg, CV_BGR2YCrCb);
	inRange(tmpimg, Scalar(0, 0, 0), Scalar(220, 220, 220), mask);
	for(int ty = 0; ty < picture.getFrame().height; ty++){
		int tx = 0 ;
		while(tx < picture.getFrame().width-1){
			if(mask.at<uchar>(ty, tx+1) == 0 && mask.at<uchar>(ty, tx) == 0)
			{ 
				maskclear.at<uchar>(ty, tx) = mask.at<uchar>(ty, tx);
				tx++; 
			}else {
				tx=picture.getFrame().width;
			}
		}
		tx = picture.getFrame().width-1;
		while(tx>1){
			if(mask.at<uchar>(ty, tx-1) == 0 && mask.at<uchar>(ty, tx) == 0)
			{
				maskclear.at<uchar>(ty,tx) = mask.at<uchar>(ty,tx);
				tx--;
			}
			else {
				tx = 0;
			}
		}
	}
	Mat element(3, 3, CV_8U, cv::Scalar(1));
	Mat elementerode(7, 7, CV_8U, cv::Scalar(1));
	erode(maskclear, maskdilate[0], elementerode);

	for(int j = 0; j < framenum; j++) {
		dilate(maskdilate[j], maskdilate[j+1], element);
		GaussianBlur(maskdilate[j], maskblur[j], Size(25, 25), 500000, 500000);

	}

	if(color <= framenum){
	for(int ty = 0; ty < picture.getFrame().height; ty++){
		int tx = 0;
		while(tx < picture.getFrame().width-1){
			if(mask.at<uchar>(ty, tx+1) == 0 && mask.at<uchar>(ty, tx) == 0) {
				tempPicture.at<Vec3b>(ty, tx)[0] = maskblur[color].at<uchar>(ty, tx);
				tempPicture.at<Vec3b>(ty, tx)[1] = maskblur[color].at<uchar>(ty, tx);
				tempPicture.at<Vec3b>(ty, tx)[2] = maskblur[color].at<uchar>(ty, tx);
				tx++;
			}
			else {
				tx = picture.getFrame().width;
			}
		}
		tx = picture.getFrame().width-1;
		while(tx > 1){
			if(mask.at<uchar>(ty, tx-1) == 0 && mask.at<uchar>(ty, tx) == 0)
			{
				tempPicture.at<Vec3b>(ty, tx)[0] = maskblur[color].at<uchar>(ty, tx);
				tempPicture.at<Vec3b>(ty, tx)[1] = maskblur[color].at<uchar>(ty, tx);
				tempPicture.at<Vec3b>(ty, tx)[2] = maskblur[color].at<uchar>(ty, tx);
				tx--;
			}else {
				tx = 0;
			}
		}
	}
	}

	picture.setContent(tempPicture);

}