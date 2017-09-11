#include <iostream>
#include <chrono>

#include "VideoPool.h"
#include "VideoRenderer.h"
#include "Director.h"

#include "WebcamSmileVideoProcessor.h"
#include "DebugSmileVideoProcessor.h"
#include "DebugHeadPoseTracker.h"

#include "vld.h"

int main()
{
	VideoPool::GetInstance();

	//SmileVideoProcessor *smileVideoProcessor = WebcamSmileVideoProcessor::GetInstance();
	SmileVideoProcessor *smileVideoProcessor = DebugSmileVideoProcessor::GetInstance();
	HeadPoseTracker *headPoseTracker = new DebugHeadPoseTracker();

	//VideoRenderer *videoRenderer = VideoRenderer::GetInstance();

	Director *director = new Director(NULL, smileVideoProcessor, headPoseTracker);
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