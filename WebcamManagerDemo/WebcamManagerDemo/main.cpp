/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include <iostream>
#include <thread>
#include <fstream>

#include "../../SmilingBuddha/SmilingBuddha/WebcamManager.h"
#include "SmileProcessStrategyDemo.h"

int main()
{
	SmileProcessStrategyDemo *smileProcessStrategyDemo = new SmileProcessStrategyDemo(NULL);
	WebcamManager *webcamManager = WebcamManager::GetInstance();
	webcamManager->SetSmileProcess(smileProcessStrategyDemo);

	webcamManager->Start();
	//webcamManager->Stop();
	//webcamManager->Start();

	while (true) {
		if (smileProcessStrategyDemo->img) {
			cv::imshow("Smile Image", *smileProcessStrategyDemo->img);
			cv::waitKey(1);
		}
	}

	return 0;
}