/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "IntroState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "BlendingTransitionVideo.h"

#include "SmileState.h"

IntroState::IntroState(Director *director)
	:InteractionState(director)
{
	this->videoPool = VideoPool::GetInstance();
	Setting *setting = Setting::GetInstance();
	this->rowCount = setting->GetRow();
	this->colCount = setting->GetCol();
	this->centerRow = setting->GetCenterRow();
	this->centerCol = setting->GetCenterCol();

	this->startTime = std::chrono::high_resolution_clock::now();
	this->isRecord = false;
	this->recordedImages = NULL;
}

IntroState::~IntroState()
{
}

void IntroState::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	director->SetInteractionState(std::make_shared<SmileState>(director, images));
}

std::shared_ptr<Video> IntroState::GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		VIDEO_TIME, loop, reverse);

	return newVideo;
}

void IntroState::SetBlendingVideo(int row, int col, std::shared_ptr<Video> newVideo)
{
	std::shared_ptr<Video> blendingVideo = std::make_shared<BlendingTransitionVideo>(
		director->GetVideoGrid()->GetChild(row, col)->GetVideo(),
		newVideo,
		BLENDING_TIME
		);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}

