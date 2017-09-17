#include <iostream>
#include <chrono>

#include "VideoPool.h"
#include "VideoRenderer.h"
#include "Director.h"

#include "WebcamSmileVideoProcessor.h"
#include "DebugSmileVideoProcessor.h"
#include "DebugHeadPoseTracker.h"

#include <portaudio.h>

//#include "vld.h"

#include "Audio.h"
#include "AudioPlayer.h"

int main()
{

	Audio audio("D:\\bell.wav");
	Audio audio1("D:\\background.wav");

	AudioPlayer player;


	player.PlayAudio(std::make_shared<Audio>(audio1));


	Sleep(1000);

	player.PlayBackgroundAudio(std::make_shared<Audio>(audio));


	Sleep(10000000);
	/*
	VideoPool::GetInstance();

	SmileVideoProcessor *smileVideoProcessor = DebugSmileVideoProcessor::GetInstance();
	HeadPoseTracker *headPoseTracker = new DebugHeadPoseTracker();

	Director *director = new Director(NULL, smileVideoProcessor, headPoseTracker);
	

	while (true) {
		std::shared_ptr<cv::Mat> img = director->GetVideoGrid()->GetFrame();
		if (img) {
			cv::imshow("Smile", *img);
			cv::waitKey(1);
		}
	}
	*/
	return 0;
}