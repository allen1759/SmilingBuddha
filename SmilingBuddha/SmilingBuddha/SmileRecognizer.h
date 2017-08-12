/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_RECOGNIZER_H
#define _SMILE_RECOGNIZER_H

#include "opencv2/core/core.hpp"
#include "opencv2/objdetect.hpp"
#include "liblinear-2.11/linear.h"

class SmileRecognizer {
public:
	/**
	* Constructor that initialize featureDescriptor and recognitionModel.
	*
	*/
	SmileRecognizer();

	~SmileRecognizer();

	/**
	* Return recognition value of the smile expression of the input face image.
	* To ensure the result is correct, the faceImage must be cropped that it only contain a face.
	* The return value is depended on the model.
	* For classification model, it return 1.0 for smile and -1.0 for nonsmile.
	* For regression model, it return the confidence value in [0, 1], 1 is the strongest.
	*
	* @param faceImage	a OpenCV Mat object for the face image.
	* @return recognition value.
	*
	*/
	double Recognize(const cv::Mat faceImage);

private:
	// SmileRecognizer constants

	// Recognition image size
	const int IMAGE_WIDTH = 100;
	const int IMAGE_HEIGHT = 100;

	const int FEATURE_DIMENSION = 576;

	// HOG parameter
	const int HOG_WINDOW_WIDTH = 100;
	const int HOG_WINDOW_HEIGHT = 100;
	const int HOG_BLOCK_WIDTH = 40;
	const int HOG_BLOCK_HEIGHT = 40;
	const int HOG_BLOCKSTRIDE_WIDTH = 20;
	const int HOG_BLOCKSTRIDE_HEIGHT = 20;
	const int HOG_CELL_WIDTH = 20;
	const int HOG_CELL_HEIGHT = 20;
	const int HOG_NUMBER_OF_BINS = 9;

	// LIBSVM model file
	const char *MODEL_FILENAME = "resources\\LogisticRegression_16_4.model";

	// SmileRecognizer private member
	cv::HOGDescriptor *featureDescriptor;
	struct model *recognitionModel;
};

#endif