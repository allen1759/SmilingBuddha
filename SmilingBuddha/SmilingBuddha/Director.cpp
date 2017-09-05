/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Director.h"

#include <iostream>
#include <chrono>

#include "RegularSmileProcessStrategy.h"
#include "SeeEachSmileProcessStrategy.h"

// for test
#include "IntroInitialState.h"

Director::Director(VideoRenderer* videoRenderer, SmileVideoProcessor *smileVideoProcessor)
{
	videoGrid = new VideoGrid();
	//videoRenderer->SetVideo(videoGrid);

	this->smileVideoProcessor = smileVideoProcessor;

	// Create first state.
	std::shared_ptr<InteractionState> state = std::make_shared<IntroInitialState>(this);
	SetInteractionState(state);

	running = true;
	updateThread = std::thread(&Director::UpdateLoop, this);
}

Director::~Director()
{
	running = false;
	updateThread.join();
}

void Director::UpdateLoop()
{
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::microseconds duration;

	while (running) {
		start = std::chrono::high_resolution_clock::now();
		if (state)
			state->Update();

		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		//std::cout << "UPDATE LOOP: " << duration.count() << std::endl;
	}
}

VideoGrid * Director::GetVideoGrid()
{
	return videoGrid;
}

void Director::SetInteractionState(std::shared_ptr<InteractionState> state)
{
	this->state = state;
	std::cout << state->ToString() << std::endl;
}

void Director::SetRegularSmileProcessorStrategy()
{
	smileVideoProcessor->SetSmileProcessStrategy(std::make_shared<RegularSmileProcessStrategy>(this));
}

void Director::SetSeeEachSmileProcessorStrategy(float waitTime)
{
	smileVideoProcessor->SetSmileProcessStrategy(std::make_shared<SeeEachSmileProcessStrategy>(this, waitTime));
}

void Director::StartInteraction()
{
	smileVideoProcessor->Start();
}

void Director::StopInteraction()
{
	smileVideoProcessor->Stop();
}

//SmileVideoProcessor * Director::GetSmileVideoProcessor()
//{
//	return smileVideoProcessor;
//}

void Director::OnSmile()
{
	state->OnSmile();
}

void Director::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	state->OnRecorded(images);
}

