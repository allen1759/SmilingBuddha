/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "AudioPool.h"

AudioPool *AudioPool::instance = NULL;

AudioPool::AudioPool()
{
	preludeAudio = std::make_shared<Audio>("resources\\prelude.wav");
	introAudio = std::make_shared<Audio>("resources\\intro.wav");
	mainAudio = std::make_shared<Audio>("resources\\main.wav");

	bellAudio = std::make_shared<Audio>("resources\\bell.wav");
	smileBellAudio = std::make_shared<Audio>("resources\\smile_bell.wav");
}

AudioPool::~AudioPool()
{
}

AudioPool *AudioPool::GetInstance()
{
	return instance;
}

inline std::shared_ptr<Audio> AudioPool::GetPreludeAudio()
{
	return preludeAudio;
}

inline std::shared_ptr<Audio> AudioPool::GetIntroAudio()
{
	return introAudio;
}

inline std::shared_ptr<Audio> AudioPool::GetMainAudio()
{
	return mainAudio;
}

inline std::shared_ptr<Audio> AudioPool::GetBellAudio()
{
	return bellAudio;
}

inline std::shared_ptr<Audio> AudioPool::GetSmileBellAudio()
{
	return smileBellAudio;
}
