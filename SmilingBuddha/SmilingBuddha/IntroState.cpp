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

IntroState::IntroState(Director *director)
	: InteractionState(director)
{
	this->videoPool = VideoPool::GetInstance();

	this->switchToSmileState = false;
	this->userImages = NULL;

	this->startTime = std::chrono::high_resolution_clock::now();
}

IntroState::~IntroState()
{
}

void IntroState::OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images)
{
	userImages = images;
	switchToSmileState = true;
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
		BLENDING_TIME);

	director->GetVideoGrid()->SetChild(blendingVideo, row, col);
}

