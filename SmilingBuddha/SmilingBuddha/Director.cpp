/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Director.h"

#include <iostream>
#include <chrono>

Director::Director(VideoRenderer* videoRenderer)
{
	videoGrid = new VideoGrid(4, 9);
	videoRenderer->SetVideo(videoGrid);

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
		std::cout << "UPDATE LOOP: " << duration.count() << std::endl;
	}
}

VideoGrid * Director::GetVideoGrid()
{
	return videoGrid;
}

void Director::SetInteractionState(InteractionState * state)
{
	this->state = state;
}
