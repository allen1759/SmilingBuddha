/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include <iostream>
#include <thread>
#include <fstream>

#include "../../smilingbuddha/smilingbuddha/webcammanager.h"
#include "smileprocessdemo.h"

int main()
{
	SmileProcessDemo *smileProcessDemo = new SmileProcessDemo(NULL);
	WebcamManager *webcamManager = WebcamManager::GetInstance();
	webcamManager->SetSmileProcess(smileProcessDemo);

	webcamManager->Start();
	//webcamManager->Stop();
	//webcamManager->Start();

	while (true) {
		if (smileProcessDemo->img) {
			cv::imshow("Smile Image", *smileProcessDemo->img);
			cv::waitKey(1);
		}
	}

	return 0;
}