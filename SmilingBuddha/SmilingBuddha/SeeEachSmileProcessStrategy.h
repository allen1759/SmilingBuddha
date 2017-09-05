/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SEE_EACH_SMILE_PROCESS_STRATEGY_H
#define _SEE_EACH_SMILE_PROCESS_STRATEGY_H

#include "SmileProcessStrategy.h"

#include <memory>
#include <vector>
#include <chrono>

#include <opencv2/core.hpp>

class SeeEachSmileProcessStrategy : public SmileProcessStrategy
{
public:
	SeeEachSmileProcessStrategy(SmileObserver *observer, float waitTime);

	~SeeEachSmileProcessStrategy();

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;
	
private:
	void SelectBestSmile();


	// For select the best smile in past.
	bool hasAnySmile;
	std::chrono::milliseconds waitTime;
	std::chrono::high_resolution_clock::time_point startTime;
	std::vector<std::shared_ptr<cv::Mat>> allImageBuffer;
	std::vector<double> allIntensityBuffer;

	bool isRecord;
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> imageBuffer;
};

#endif // !_SEE_EACH_SMILE_PROCESS_STRATEGY_H