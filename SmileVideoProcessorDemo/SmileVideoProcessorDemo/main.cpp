/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include <iostream>
#include <thread>
#include <fstream>
#include <conio.h>

#include "../../SmilingBuddha/SmilingBuddha/WebcamSmileVideoProcessor.h"
#include "../../SmilingBuddha/SmilingBuddha/DebugSmileVideoProcessor.h"
#include "SmileProcessStrategyDemo.h"
#include "../../SmilingBuddha/SmilingBuddha/RegularSmileProcessStrategy.h"

int main()
{
	SmileProcessStrategy *smileProcessStrategy = new SmileProcessStrategyDemo(NULL);
	//SmileProcessStrategy *smileProcessStrategy = new RegularSmileProcessStrategy(NULL, 40);

	SmileVideoProcessor *smileVideoProcessor = WebcamSmileVideoProcessor::GetInstance();
	//SmileVideoProcessor *smileVideoProcessor = DebugSmileVideoProcessor::GetInstance();
	smileVideoProcessor->SetSmileProcessStrategy(smileProcessStrategy);

	smileVideoProcessor->Start();

	char ch;
	while (true) {
		std::shared_ptr<cv::Mat> img = ((SmileProcessStrategyDemo *)smileProcessStrategy)->img;
		if (img) {
			cv::imshow("test", *img);
			cv::waitKey(1);
		}
	}

	return 0;
}