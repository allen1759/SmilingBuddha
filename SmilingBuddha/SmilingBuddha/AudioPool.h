/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _AUDIO_POOL_H
#define _AUDIO_POOL_H

#include <memory>

#include "Audio.h"

class AudioPool
{
private:
	AudioPool();

public:
	~AudioPool();

	static AudioPool * GetInstance();

	std::shared_ptr<Audio> GetPreludeAudio();

	std::shared_ptr<Audio> GetIntroAudio();

	std::shared_ptr<Audio> GetMainAudio();

	std::shared_ptr<Audio> GetBellAudio();

	std::shared_ptr<Audio> GetSmileBellAudio();

private:

	static AudioPool *instance;

	// Backgound audio resources.
	std::shared_ptr<Audio> preludeAudio;
	std::shared_ptr<Audio> introAudio;
	std::shared_ptr<Audio> mainAudio;

	// Effect audio resources.
	std::shared_ptr<Audio> bellAudio;
	std::shared_ptr<Audio> smileBellAudio;
};


#endif // !_AUDIO_POOL_H
