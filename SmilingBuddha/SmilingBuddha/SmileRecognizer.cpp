/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SmileRecognizer.h"

#include <iostream>
#include <vector>
#include "opencv2/imgproc.hpp"

SmileRecognizer::SmileRecognizer()
{
	// initialize HOG feature descirptor
	featureDescriptor = new cv::HOGDescriptor(cv::Size(HOG_WINDOW_WIDTH, HOG_WINDOW_HEIGHT),
		cv::Size(HOG_BLOCK_WIDTH, HOG_BLOCK_HEIGHT),
		cv::Size(HOG_BLOCKSTRIDE_WIDTH, HOG_BLOCKSTRIDE_HEIGHT),
		cv::Size(HOG_CELL_WIDTH, HOG_CELL_HEIGHT), HOG_NUMBER_OF_BINS);

	// initialize SVM recognition model
	recognitionModel = load_model(MODEL_FILENAME);

	if (!recognitionModel)
		std::cerr << "Cannot Read Model File";
	// ---------------------------throw exp------------------------------
}

SmileRecognizer::~SmileRecognizer()
{
	delete featureDescriptor;
	delete recognitionModel;
}

double SmileRecognizer::Recognize(const cv::Mat faceImage) {
	// if image is empty return false
	if (faceImage.empty()) {
		return -1;
	}

	// preprocess input image
	cv::Mat recognitionImage;
	// TODO: if color image, convert
	//cv::cvtColor(faceImage, recognitionImage, CV_BGR2GRAY);
	cv::resize(faceImage, recognitionImage, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	// extract features
	std::vector<float> descriptorValue;
	featureDescriptor->compute(recognitionImage, descriptorValue, cv::Size(24, 24));

	// copy the descriptor value to LIBSVM structure
	struct feature_node *featureNode = new feature_node[FEATURE_DIMENSION + 1];
	for (int i = 0; i < FEATURE_DIMENSION; ++i) {
		featureNode[i].value = descriptorValue[i];
		featureNode[i].index = i + 1;
	}
	featureNode[FEATURE_DIMENSION].value = 0;
	featureNode[FEATURE_DIMENSION].index = -1;

	// do recognition
	double *probabilities = new double[2];
	predict_probability(recognitionModel, featureNode, probabilities);

	double reconitionValue = probabilities[0];

	// release memory
	recognitionImage.release();
	descriptorValue.clear();
	delete featureNode;
	delete probabilities;

	return reconitionValue;
}