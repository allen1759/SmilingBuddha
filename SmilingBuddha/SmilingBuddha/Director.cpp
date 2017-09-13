/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Director.h"

#include <iostream>
#include <chrono>

#include "OnSmileEvent.h"
#include "OnRecordedEvent.h"
#include "RegularSmileProcessStrategy.h"
#include "SeeEachSmileProcessStrategy.h"
#include "PreludeInitialState.h"

// for test
#include "IntroInitialState.h"

Director::Director(VideoRenderer* videoRenderer, SmileVideoProcessor *smileVideoProcessor, HeadPoseTracker *headPoseTracker)
{
	this->videoGrid = new VideoGrid();
	//videoRenderer->SetVideo(videoGrid);

	this->userImageSequenceRecords = std::make_shared<std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>>();

	this->smileVideoProcessor = smileVideoProcessor;
	this->headPoseTracker = headPoseTracker;

	// Create first state.
	//std::shared_ptr<InteractionState> state = std::make_shared<IntroInitialState>(this);
	std::shared_ptr<InteractionState> state = std::make_shared<PreludeInitialState>(this);
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
		if (state) {
			state->Update();
			eventQueueMutex.lock();
			while (!eventQueue.empty()) {
				std::shared_ptr<Event> e = eventQueue.front();
				eventQueue.pop();
				state->SignalEvent(e);
			}
			eventQueueMutex.unlock();
		}

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
	headPoseTracker->StartTracking();
}

void Director::StopInteraction()
{
	smileVideoProcessor->Stop();
	headPoseTracker->StopTracking();
}

std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>> Director::GetUserImageSequenceRecords()
{
	return userImageSequenceRecords;
}

Ray Director::GetHeadPose()
{
	return headPoseTracker->GetHeadPose();
}

void Director::OnSmile()
{
	eventQueueMutex.lock();
	eventQueue.push(std::make_shared<OnSmileEvent>());
	eventQueueMutex.unlock();
}

void Director::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	eventQueueMutex.lock();
	eventQueue.push(std::make_shared<OnRecordedEvent>(images));
	eventQueueMutex.unlock();

	userImageSequenceRecords->push_back(images);
}

void Director::ClearUserImageSequenceRecords()
{
	userImageSequenceRecords->clear();
}
