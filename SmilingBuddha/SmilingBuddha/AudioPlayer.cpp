#include "AudioPlayer.h"
/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include <iostream>

AudioPlayer::AudioPlayer()
{
	// Initilize Portaudio.
	PaError error = Pa_Initialize();
	if (error != paNoError)
		std::cerr << "PortAudio Initilize Error: " << Pa_GetErrorText(error) << std::endl;

	// Fill in output stream parameter.
	outputParameters.device = Pa_GetDefaultOutputDevice();
	
	outputParameters.channelCount = 2;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	
	// Open the audio stream.
	error = Pa_OpenStream(&stream, NULL, &outputParameters, 44100, paFramesPerBufferUnspecified, paNoFlag, StaticPlayAudio, this);
	
	if (error != paNoError)
		std::cerr << "Error open ouput audio stream: " << Pa_GetErrorText(error) << std::endl;

	channelCount = outputParameters.channelCount;

	audioList = std::make_shared<std::vector<std::shared_ptr<Audio>>>();

	Pa_StartStream(stream);
}

AudioPlayer::~AudioPlayer()
{
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
}

void AudioPlayer::PlayAudio(std::shared_ptr<Audio> audio)
{
	audio->Reset();
	audioListMutex.lock();
	audioList->push_back(audio);
	audioListMutex.unlock();
}

void AudioPlayer::PlayBackgroundAudio(std::shared_ptr<Audio> audio)
{
	audio->Reset();
	backgroundAudioMutex.lock();
	backgroundAudio = audio;
	backgroundAudioMutex.unlock();
}


int AudioPlayer::PlayAudio(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags)
{
	float *outputBufferPointer = (float *)outputBuffer;

	for (unsigned int i = 0; i < framesPerBuffer; i++) {
		int frameIndex = i * channelCount;


		audioListMutex.lock();

		for (std::vector<std::shared_ptr<Audio>>::iterator it = audioList->begin(); it != audioList->end(); ) {
			if ((*it)->IsEnd())
				it = audioList->erase(it);
			else
				++it;
		}
		
		std::vector<std::shared_ptr<std::vector<float>>> frameBuffer;
		for (int j = 0; j < audioList->size(); ++j) {
			frameBuffer.push_back(audioList->at(j)->GetFrame());
		}
		audioListMutex.unlock();

		backgroundAudioMutex.lock();
		if (backgroundAudio) {
			if (backgroundAudio->IsEnd())
				backgroundAudio->Reset();

			frameBuffer.push_back(backgroundAudio->GetFrame());
		}
		backgroundAudioMutex.unlock();

		for (int j = 0; j < channelCount; ++j)
			outputBufferPointer[frameIndex + j] = 0.0f;

		for (int j = 0; j < channelCount; ++j) {
			for (int k = 0; k < frameBuffer.size(); ++k) {
				std::shared_ptr<std::vector<float>> frame = frameBuffer.at(k);
				outputBufferPointer[frameIndex + j] += frame->at(j % frame->size());
			}
		}
		
	}

	return paContinue;
}