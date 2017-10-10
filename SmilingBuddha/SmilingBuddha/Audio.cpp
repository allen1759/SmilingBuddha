/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Audio.h"

Audio::Audio(std::string fileName)
{
	SndfileHandle fileHandle(fileName, SFM_READ);

	frameCount = fileHandle.frames();
	channelCount = fileHandle.channels();
	
	frames = new float[frameCount * channelCount];
	
	fileHandle.readf(frames, frameCount);

	frameIndex = 0;
	end = false;
}

Audio::~Audio()
{
	delete[] frames;
}

void Audio::Reset()
{
	frameIndex = 0;
	end = false;
}

bool Audio::IsEnd()
{
	return end;
}

std::shared_ptr<std::vector<float>> Audio::GetFrame()
{
	if (frameIndex >= frameCount - 1) {
		frameIndex = frameCount - 1;
		end = true;
	}
	

	std::shared_ptr<std::vector<float>> frame = std::make_shared<std::vector<float>>();

	for (int i = 0; i < channelCount; ++i)
		frame->push_back(frames[frameIndex * channelCount + i]);
	
	frameIndex++;

	return frame;
}

