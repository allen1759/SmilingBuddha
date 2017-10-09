/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "AudioPool.h"

AudioPool *AudioPool::instance = NULL;

AudioPool::AudioPool()
{
	preludeAudio = std::make_shared<Audio>("resources\\audio\\prelude.wav");
	introAudio = std::make_shared<Audio>("resources\\audio\\intro.wav");
	mainAudio = std::make_shared<Audio>("resources\\audio\\main.wav");

	bellAudio = std::make_shared<Audio>("resources\\audio\\bell.wav");
	smileBellAudio = std::make_shared<Audio>("resources\\audio\\smile_bell.wav");
}

AudioPool::~AudioPool()
{
}

AudioPool *AudioPool::GetInstance()
{
	if (instance == NULL)
		instance = new AudioPool();

	return instance;
}

std::shared_ptr<Audio> AudioPool::GetPreludeAudio()
{
	return std::make_shared<Audio>(*preludeAudio);
}

std::shared_ptr<Audio> AudioPool::GetIntroAudio()
{
	return std::make_shared<Audio>(*introAudio);
}

std::shared_ptr<Audio> AudioPool::GetMainAudio()
{
	return std::make_shared<Audio>(*mainAudio);
}

std::shared_ptr<Audio> AudioPool::GetBellAudio()
{
	return std::make_shared<Audio>(*bellAudio);
}

std::shared_ptr<Audio> AudioPool::GetSmileBellAudio()
{
	return std::make_shared<Audio>(*smileBellAudio);
}
