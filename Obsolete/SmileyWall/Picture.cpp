/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "Picture.h"
#include "AnimationFactory.h"

using namespace cv;

void Picture::playAnimation()
{
	if (getTickCount() < enableAnimationTickCount)
		return;

	for (vector<Animation*>::iterator animation = animationList.begin(); animation != animationList.end(); ++animation)
	{
		if (!(*animation)->animationEnded())
			(*animation)->play(*this);
	}
}

void Picture::setAnimation(const AnimationEnum &animationEnum, 
	const cv::InputArray &startImg, const cv::InputArray &endImg, bool clear)
{
	if (clear)
	{
		clearAnimation();
	}

	Mat startImage = startImg.getMat();
	Mat endImage = endImg.getMat();
	if (!startImg.empty() && startImg.size() != this->size())
		cv::resize(startImg, startImage, this->size()), 0, 0, INTER_AREA;
	if (!endImg.empty() && endImg.size() != this->size() && animationEnum != ScaleAnimationEnum)
		cv::resize(endImg, endImage, this->size(), 0, 0, INTER_AREA);

	Animation *animation;
	animation = AnimationFactory::createAnimation(animationEnum, startImage, endImage);
	animation->setEnd(false);
	if (animationList.empty())
	{
		animationList.push_back(animation);
	} else {
		animationList[0] = animation;
	}
}

void Picture::addAnimation(const AnimationEnum &animationEnum, 
	const cv::InputArray &startImg, const cv::InputArray &endImg)
{
	Mat startImage = startImg.getMat();
	Mat endImage = endImg.getMat();
	if (!startImg.empty() && startImg.size() != this->size())
		cv::resize(startImg, startImage, this->size(), 0, 0, INTER_AREA);
	if (!endImg.empty() && endImg.size() != this->size())
		cv::resize(endImg, endImage, this->size(), 0, 0, INTER_AREA);

	Animation *animation;
	animation = AnimationFactory::createAnimation(animationEnum, startImage, endImage);
	animation->setEnd(false);
	animationList.push_back(animation);
}

void Picture::setAnimation(const AnimationEnum &animationEnum, 
	const CvArr *startImg, const CvArr *endImg, bool clear)
{
	if (clear)
	{
		clearAnimation();
	}
	
	Animation *animation;
	animation = AnimationFactory::createAnimation(animationEnum, 
		cvarrToMat(startImg), cvarrToMat(endImg));
	if (animationList.empty())
	{
		animationList.push_back(animation);
	} else {
		animationList[0] = animation;
	}
}

void Picture::addAnimation(const AnimationEnum &animationEnum, 
	const CvArr *startImg, const CvArr *endImg)
{
	Animation *animation;
	animation = AnimationFactory::createAnimation(animationEnum, 
		cvarrToMat(startImg), cvarrToMat(endImg));
	animationList.push_back(animation);
}

void Picture::setContent(const InputArray &image)
{
	if (image.size() == this->size()) {
		image.getMat().convertTo(*this, this->getType());
		return;
	}

	Mat resizeImage;
	cv::resize(image, resizeImage, this->size(), 0, 0, INTER_AREA);
	resizeImage.convertTo(*this, this->getType());
}

void Picture::setContent(const CvArr* image)
{
	setContent(cv::cvarrToMat(image));
}

void Picture::setFrame(const cv::InputArray &roi, const cv::Rect &rect)
{
	Mat origin;
	this->copyTo(origin);
	setContent(zeros(frame.height, frame.width, getType()));
	this->data = roi.getMat().data;
	this->rows = rect.height;
	this->cols = rect.width;
	setContent(origin);
	frame = rect;
}