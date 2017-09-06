#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "WebcamSmileVideoProcessor.h"
#include "DebugSmileVideoProcessor.h"
#include "VideoRenderer.h"
#include "Director.h"
#include "IntroInitialState.h"


#include "vld.h" 
int main()
{
	VideoPool::GetInstance();

	//SmileVideoProcessor *smileVideoProcessor = WebcamSmileVideoProcessor::GetInstance();
	SmileVideoProcessor *smileVideoProcessor = DebugSmileVideoProcessor::GetInstance();
	//VideoRenderer *videoRenderer = VideoRenderer::GetInstance();

	Director *director = new Director(NULL, smileVideoProcessor);
	//Director *director = new Director(videoRenderer, smileVideoProcessor);


	while (true) {
		std::shared_ptr<cv::Mat> img = director->GetVideoGrid()->GetFrame();
		if (img) {
			cv::imshow("Smile", *img);
			cv::waitKey(1);
		}
	}
	//videoRenderer->RenderLoop();

	return 0;
}