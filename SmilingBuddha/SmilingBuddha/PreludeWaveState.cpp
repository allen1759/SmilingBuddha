#include "PreludeWaveState.h"

#include "PreludeWave2LeftState.h"
#include "PreludeWave2RightState.h"
#include "PreludeWave2UpState.h"
#include "PreludeWave2BottomState.h"
#include "PreludeWave2RandomState.h"

std::shared_ptr<PreludeWaveState> PreludeWaveState::CreateRandomWaveState(Director *director, bool isFirstWave)
{
	int waveIndex = rand() % PRELUDE_WAVE_STATE_COUNT;

	switch (waveIndex)
	{
	case 0:
		return std::make_shared<PreludeWave2LeftState>(director, isFirstWave);
		break;

	case 1:
		return std::make_shared<PreludeWave2RightState>(director, isFirstWave);
		break;

	case 2:
		return std::make_shared<PreludeWave2UpState>(director, isFirstWave);
		break;

	case 3:
		return std::make_shared<PreludeWave2BottomState>(director, isFirstWave);
		break;

	case 4:
		return std::make_shared<PreludeWave2RandomState>(director, isFirstWave);
		break;

	default:
		return NULL;
		break;
	}
}

inline void PreludeWaveState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - PreludeState::startTime).count();

	if (elapsedTime > PRELUDE_WAVE_STATE_TIME) {
		if (isFirstWave) {
			director->SetInteractionState(PreludeWaveState::CreateRandomWaveState(director, false));
			return;
		}
		else {
			for (int row = 0; row < Setting::GetInstance()->GetRow(); ++row) {
				for (int col = 0; col < Setting::GetInstance()->GetCol(); ++col) {
					std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(videoPool->GetSmileVideoList(row, col), PreludeState::USER_VIDEO_TIME, true, true);
					director->GetVideoGrid()->SetChild(newVideo, row, col);
				}
			}
			director->SetInteractionState(std::make_shared<PreludeBuddhaState>(director));
			return;
		}
	}
	else if (elapsedTime > nextAppearTime) {
		AppeartNext();
	}
}
