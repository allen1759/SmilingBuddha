/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _DIRECTOR_H
#define _DIRECTOR_H

#include "UserObserver.h"
#include "SmileObserver.h"

#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

#include "VideoRenderer.h"
#include "VideoGrid.h"
#include "Event.h"
#include "InteractionState.h"
// Controllers
#include "SmileVideoProcessor.h"
#include "HeadPoseTracker.h"
#include "UserDetector.h"

class Director : public UserObserver, public SmileObserver
{
public:
	Director(VideoRenderer* videoRenderer, SmileVideoProcessor *smileVideoProcessor, HeadPoseTracker *headPoseTracker, UserDetector *userDetector);

	~Director();

	void UpdateLoop();

	std::shared_ptr<VideoGrid> GetVideoGrid();

	// ********** If this function called in state's Update(), make   **********
	// ********** sure to 'retrun;' immediately after function call.  **********
	void SetInteractionState(std::shared_ptr<InteractionState> state);

	void SetRegularSmileProcessorStrategy();

	void SetSeeEachSmileProcessorStrategy(float waitTime);

	void StartInteraction();

	void StopInteraction();

	std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>> GetUserImageSequenceRecords();

	Ray GetHeadPose();

	// Implement UserObserver's pure virtual function.
	virtual void OnUserDetect() override;

	virtual void OnUserLeave() override;

	// Implement SmileObserver's pure virtual function.
	virtual void OnSmile() override;

	virtual void OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images) override;

private:
	std::shared_ptr<VideoGrid> videoGrid;

	std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>>> userImageSequenceRecords;

	void CheckKeyboardTrigger();

	// Controllers.
	SmileVideoProcessor *smileVideoProcessor;
	HeadPoseTracker *headPoseTracker;

	bool running;
	std::thread updateThread;
	std::shared_ptr<InteractionState> state;

	std::queue<std::shared_ptr<Event>> eventQueue;
	std::mutex eventQueueMutex;
};

#endif