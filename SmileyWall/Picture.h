/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef PICTURE_H
#define PICTURE_H

#include <opencv2/opencv.hpp>
#include <stdint.h>
#include "AllAnimations.h"



class Animation;

/*!
 * \brief
 * A basic photo class.
 * 
 * Picture class is one-to-one corresponding to a photo in the painting. \n
 It plays animation itself.
 */
class Picture: public cv::Mat
{
public:
	Picture(const cv::InputArray &image, const cv::Rect &rect): cv::Mat(image.getMat()), 
		enableAnimationTickCount(INT64_MIN), frame(rect){}
	Picture(const CvArr* image, const cv::Rect &rect): cv::Mat(cv::cvarrToMat(image)), 
		enableAnimationTickCount(INT64_MIN), frame(rect){}

	~Picture() {
		clearAnimation();
	}
	/*!
	*  \brief Play animation itself at time you set.
	*/
	void playAnimation();

	/*!
	*  \brief A setter for animation.
		\param animationEnum specify which animation you want to apply.
		\param startImg shown image after the animation starts.
		\param endImg shown image after the animation is end.
	*/
	void setAnimation(const AnimationEnum &animationEnum, 
		const cv::InputArray &startImg, 
		const cv::InputArray &endImg = cv::Mat::zeros(1, 1, CV_8UC3), bool clear=true);

	/*!
	*  \brief A setter for animation.
		\param animationEnum specify which animation you want to apply.
		\param startImg shown image after the animation starts.
		\param endImg shown image after the animation is end.
	*/
	void addAnimation(const AnimationEnum &animationEnum, 
		const cv::InputArray &startImg, 
		const cv::InputArray &endImg = cv::Mat::zeros(1, 1, CV_8UC3));

	/*!
	*  \brief A setter for animation.
		\param animationEnum specify which animation you want to apply.
		\param startImg shown image after the animation starts.
		\param endImg shown image after the animation is end.
	*/
	void setAnimation(const AnimationEnum &animationEnum, 
		const CvArr *startImg, const CvArr *endImg, bool clear=true);

	/*!
	*  \brief A setter for animation.
		\param animationEnum specify which animation you want to apply.
		\param startImg shown image after the animation starts.
		\param endImg shown image after the animation is end.
	*/
	void addAnimation(const AnimationEnum &animationEnum, 
		const CvArr *startImg, const CvArr *endImg);

	void clearAnimation() {
		for( std::vector<Animation*>::iterator i = animationList.begin(); i != animationList.end(); ++i )
		{
			delete *i;
		}
		animationList.clear();
		std::vector<Animation*> t;
		animationList.swap(t);
	}

	void deleteAnimation(int index) {
		animationList.erase(animationList.begin()+index);
	}

	/*!
	*  \brief A getter for animation.
		\return an animation pointer.
	*/
	inline Animation* getAnimation(int index=0) const {
		if (static_cast<unsigned>(index) >= animationList.size())
		{
			return NULL;
		}
		return animationList.at(index);
	}

	inline Animation* getLastAnimation() const {
		if (animationList.empty())
		{
			return NULL;
		}
		return animationList.back();
	}

	inline void popAnimation() {
		animationList.pop_back();
	}

	inline int getAnimationSize() const {return static_cast<int>(animationList.size());}

	/*!
	*  \brief A setter for animation time.
		\param enableTime animation time you want to play.
	*/
	inline void setEnableAnimationTime(const int64 &enableTime) {
		enableAnimationTickCount = enableTime;
	}

	/*!
	*  \brief A setter for picture.
		\param image shown image.
	*/
	void setContent(const cv::InputArray &image);
	
	/*!
	*  \brief A setter for picture.
		\param image shown image.
	*/
	void setContent(const CvArr* image);

	/*!
	*  \brief A getter for frame.
		\return the location and size of this picture. \n
		note: the location is the upper left of this picture.
	*/
	inline cv::Rect getFrame() const {return frame;}

	/*!
	*  \brief A setter for frame.
		\param roi region of interesting.
		\param rect contains the location and size of this picture. \n
		note: the location is the upper left of this picture.
	*/
	void setFrame(const cv::InputArray &roi, const cv::Rect &rect);

	/*!
	*  \brief A setter for frame.
		\param roi region of interesting. 
		\param rect contains the location and size of this picture. \n
		note: the location is the upper left of this picture.
	*/
	void setFrame(const CvArr* roi, const cv::Rect &rect);

	/*!
	*  \brief A getter for picture data type.
		\return picture data type
	*/
	inline int getType() const {return this->type();}

	/*!
	*  \brief A getter for image data.
		\return a pointer to image data.
	*/
	inline uchar* getData() const {return this->data;}
private:
	//Animation *animation;
	std::vector<Animation*> animationList;
	int64 enableAnimationTickCount;
	cv::Rect frame;
};

#endif