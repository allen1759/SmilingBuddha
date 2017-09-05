/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _DIRECTOR_H
#define _DIRECTOR_H

#include "SmileObserver.h"

#include <thread>

#include "VideoRenderer.h"
#include "VideoGrid.h"
#include "InteractionState.h"
#include "SmileVideoProcessor.h"

class Director : public SmileObserver
{
public:
	//TODO: Add controller
	Director(VideoRenderer* videoRenderer, SmileVideoProcessor *smileVideoProcessor);

	~Director();

	void UpdateLoop();

	VideoGrid *GetVideoGrid();

	void SetInteractionState(std::shared_ptr<InteractionState> state);

	void SetRegularSmileProcessorStrategy();

	void SetSeeEachSmileProcessorStrategy(float waitTime);

	void StartInteraction();

	void StopInteraction();

	//SmileVideoProcessor *GetSmileVideoProcessor();

	// Inherit from SmileObserver.
	virtual void OnSmile() override;

	virtual void OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images) override;

private:
	VideoGrid *videoGrid;

	bool running;
	std::thread updateThread;
	std::shared_ptr<InteractionState> state;

	// Controllers
	SmileVideoProcessor *smileVideoProcessor;

};

#endif