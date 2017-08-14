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

int main()
{
	SmileProcessStrategyDemo *smileProcessStrategyDemo = new SmileProcessStrategyDemo(NULL);
	//SmileVideoProcessor *smileVideoProcessor = WebcamSmileVideoProcessor::GetInstance();
	SmileVideoProcessor *smileVideoProcessor = DebugSmileVideoProcessor::GetInstance();
	smileVideoProcessor->SetSmileProcessStrategy(smileProcessStrategyDemo);

	smileVideoProcessor->Start();
	//smileVideoProcessor->Stop();
	//smileVideoProcessor->Start();

	char ch;
	while (true) {
		if (kbhit()) {
			ch = _getch();
			if (ch == 's' || ch == 'S')
				((DebugSmileVideoProcessor *)smileVideoProcessor)->StartSmile();
			else if (ch == 'u' || ch == 'U')
				((DebugSmileVideoProcessor *)smileVideoProcessor)->StopSmile();
		}
		if (smileProcessStrategyDemo->img) {
			cv::imshow("Smile Image", *smileProcessStrategyDemo->img);
			cv::waitKey(1);
		}
	}

	return 0;
}