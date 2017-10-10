#include "PreludeBuddhaState.h"

void PreludeBuddhaState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - PreludeState::startTime).count();

	if (elapsedTime > PRELUDE_BUDDHA_STATE_TIME) {
		VideoRenderer::GetInstance()->SetVideo(director->GetVideoGrid());
		if (buddhaStateCount == MAX_BUDDHA_STATE_COUNT) {
			// Choose one wave randomly.
			director->SetInteractionState(PreludeWaveState::CreateRandomWaveState(director, true));
			return;
		}
		else
			director->SetInteractionState(std::make_shared<PreludeBuddhaState>(director, buddhaStateCount));
	}
}
