/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _AUDIO_H
#define _AUDIO_H

#include <string>
#include <memory>
#include <vector>

#include <sndfile.hh>

class Audio
{
public:
	Audio(std::string fileName);
	~Audio();

	void Reset();

	bool IsEnd();

	std::shared_ptr<std::vector<float>> GetFrame();

private:
	int frameCount;
	int channelCount;
	int frameIndex;
	float *frames;

	bool end;
};

#endif // !_AUDIO_H
