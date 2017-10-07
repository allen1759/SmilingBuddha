/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _AUDIO_PLAYER_H
#define _AUDIO_PLAYER_H

#include <memory>
#include <vector>
#include <mutex>

#include "portaudio.h"

#include "Audio.h"

// TODO: singleton


class AudioPlayer {
public:
	AudioPlayer();
	~AudioPlayer();

	void PlayAudio(std::shared_ptr<Audio> audio);

	void PlayBackgroundAudio(std::shared_ptr<Audio> audio);

private:
	static int StaticPlayAudio(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *thisPointer)
	{
		AudioPlayer* self = static_cast<AudioPlayer*>(thisPointer);
		return self->PlayAudio(outputBuffer, framesPerBuffer, timeInfo, statusFlags);
	}

	int PlayAudio(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);


	PaStream* stream;
	PaStreamParameters outputParameters;
	int channelCount;

	std::shared_ptr<std::vector<std::shared_ptr<Audio>>> audioList;
	std::mutex audioListMutex;

	std::shared_ptr<Audio> backgroundAudio;
	std::mutex backgroundAudioMutex;
};

#endif