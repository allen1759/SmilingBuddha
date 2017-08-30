/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _DIRECTOR_H
#define _DIRECTOR_H

#include <thread>

#include "VideoRenderer.h"
#include "VideoGrid.h"
#include "InteractionState.h"

class Director
{
public:
	//TODO: Add controller
	Director(VideoRenderer* videoRenderer);
	~Director();

	void UpdateLoop();

	VideoGrid *GetVideoGrid();

	void SetInteractionState(InteractionState *state);

private:
	VideoGrid *videoGrid;

	bool running;
	std::thread updateThread;
	InteractionState *state;
};

#endif