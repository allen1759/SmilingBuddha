/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "GazeState.h"

#include "Setting.h"
#include "Director.h"
#include "VideoClip.h"
#include "BroadcastState.h"

GazeState::GazeState(Director *director, std::chrono::high_resolution_clock::time_point startTime)
	: InteractionState(director),
	  ROW_COUNT(Setting::GetInstance()->GetRow()),
	  COL_COUNT(Setting::GetInstance()->GetCol()),
	ROW_CENTER(Setting::GetInstance()->GetCenterRow()),
	COL_CENTER(Setting::GetInstance()->GetCenterCol()),
	  PROJECTION_WIDTH(Setting::GetInstance()->GetProjectionWidth()),
	  PROJECTION_HEIGHT(Setting::GetInstance()->GetProjectionHeight())
{
	this->videoPool = VideoPool::GetInstance();

	this->switchToBroadcastState = false;
	this->userImages = NULL;

	this->elapsedTime = 0.0f;
	this->startTime = startTime;
}

GazeState::~GazeState()
{
}

void GazeState::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

	if (elapsedTime > GAZE_STATE_TIME)
		switchToBroadcastState = true;
}

std::shared_ptr<Video> GazeState::GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse)
{
	std::shared_ptr<ActorVideoSet> actorVideoSet = videoPool->GetActorVideoSet(row, col);
	std::shared_ptr<Video> newVideo = std::make_shared<VideoClip>(
		actorVideoSet->GetDirectionVideo(direction),
		ACTOR_VIDEO_TIME, loop, reverse);

	return newVideo;
}

void GazeState::HeadPost2RowCol(Ray headPose, int & row, int & col)
{
	glm::vec3 origin = headPose.GetOrigin();
	glm::vec3 direction = headPose.GetDirection();

	// Check boundary case.
	if (direction.z == 0)
		direction.z = 1e-9;

	float multiply = -origin.z / direction.z;
	glm::vec3 intersection = origin + direction * multiply;

	float rowMeter = -intersection.y + PROJECTION_HEIGHT / 2;
	float colMeter = intersection.x + PROJECTION_WIDTH / 2;
	std::cout << "Meter row: " << rowMeter << " col: " << colMeter << std::endl;

	row = rowMeter / PROJECTION_HEIGHT * ROW_COUNT;
	col = colMeter / PROJECTION_WIDTH * COL_COUNT;

	row = std::max(0, std::min(ROW_COUNT - 1, row));
	col = std::max(0, std::min(COL_COUNT - 1, col));
}
