/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_STATE_H
#define _SMILE_STATE_H

#include "InteractionState.h"

#include "Setting.h"
#include "VideoPool.h"
#include "Director.h"
#include "VideoClip.h"
// TODO: replace to wave transition
#include "BlendingTransitionVideo.h"

class SmileState : public InteractionState
{
public:
	SmileState(Director *director, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images);

	~SmileState();

	virtual void Update() override;

	virtual std::string ToString() override;

private:
	void SetWaveAnimation(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images);

	void SetSmileAnimation();

	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse);

	void SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo);


	// Get from Setting class.
	// todo cosnt 
	int rowCount;
	int colCount;
	int centerRow;
	int centerCol;

	// Initialize only 9 grids around center.
	const int SQUARE_SIZE = 9;
	int DIRECTION[9][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
	{ 0, -1 },{ 0, 0 },{ 0, 1 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 } };
	int NEAR_BY_DIRECTION[8][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
	{ 0, -1 },{ 0, 1 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 } };

	const float WAVE_TIME = 3.0f;
	const float SMILE_VIDEO_TIME = 6.0f;
	const float BLENDING_TIME = 0.5f;

	VideoPool *videoPool;

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::milliseconds waveDuration;
	std::chrono::milliseconds smileDuration;
};

SmileState::SmileState(Director *director, std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
	: InteractionState(director)
{
	Setting *setting = Setting::GetInstance();
	this->rowCount = setting->GetRow();
	this->colCount = setting->GetCol();
	this->centerRow = setting->GetCenterRow();
	this->centerCol = setting->GetCenterCol();

	videoPool = VideoPool::GetInstance();

	this->waveDuration = std::chrono::milliseconds(static_cast<int>(WAVE_TIME * 1000));
	this->smileDuration = std::chrono::milliseconds(static_cast<int>(SMILE_VIDEO_TIME * 1000));
	this->startTime = std::chrono::high_resolution_clock::now();

	SetWaveAnimation(images);
}

SmileState::~SmileState()
{
}

void SmileState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

	if (delta > waveDuration) {
		SetSmileAnimation();
		startTime = std::chrono::high_resolution_clock::now();
	}
	else if (delta > waveDuration + smileDuration) {
		director->SetInteractionState(NULL);
	}
}

std::string SmileState::ToString()
{
	return "SmileState";
}

void SmileState::SetWaveAnimation(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(images, SMILE_VIDEO_TIME, false, true);
	std::shared_ptr<Video> waveVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(centerRow, centerCol),
		newVideo, WAVE_TIME);

	director->GetVideoGrid()->SetChild(waveVideo, centerRow, centerCol);
}

void SmileState::SetSmileAnimation()
{
	for (int i = 0; i < SQUARE_SIZE - 1; ++i) {
		int row = centerRow + NEAR_BY_DIRECTION[i][1];
		int col = centerCol + NEAR_BY_DIRECTION[i][0];
		int lookDirection = ActorVideoSet::GetDirectionIndex(row, col, centerRow, centerCol);

		std::shared_ptr<Video> newVideo = GetActorDirectionVideo(row, col, lookDirection, false, true);
		SetBlendingVideo(row, col, newVideo);
	}
}

std::shared_ptr<Video> SmileState::GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		SMILE_VIDEO_TIME, loop, reverse);

	return newVideo;
}

void SmileState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME
		);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}



#endif // !_SMILE_STATE_H
