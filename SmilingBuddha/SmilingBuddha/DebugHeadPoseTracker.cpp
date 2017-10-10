#include "DebugHeadPoseTracker.h"

#include <iostream>
#include <conio.h>

#include <glm/gtc/quaternion.hpp>

#include "InputManager.h"

DebugHeadPoseTracker::DebugHeadPoseTracker()
{
	yaw = 0.0f;
	pitch = 0.0f;

	headPose.SetOrigin(glm::vec3(0.0f, 0.0f, 1.5f));
}

DebugHeadPoseTracker::~DebugHeadPoseTracker()
{
	StopTracking();

	if (trackingThread) {
		trackingThread->join();
		trackingThread = NULL;
	}
}

void DebugHeadPoseTracker::StartTracking()
{
	if (!isTracking) {
		if (trackingThread) {
			trackingThread->join();
			trackingThread = NULL;
		}

		isTracking = true;
		trackingThread = std::make_shared<std::thread>(&DebugHeadPoseTracker::Tracking, this);
	}
}

void DebugHeadPoseTracker::StopTracking()
{
	isTracking = false;
}

void DebugHeadPoseTracker::Tracking()
{
	while (isTracking) {
		if (InputManager::GetInstance()->GetKey('a')) {
			pitch++;
		}
		else if (InputManager::GetInstance()->GetKey('w')) {
			yaw++;
		}
		else if (InputManager::GetInstance()->GetKey('d')) {
			pitch--;
		}
		else if (InputManager::GetInstance()->GetKey('s')) {
			yaw--;
		}

		headPose.SetDirection(glm::quat(glm::vec3(glm::radians(yaw), glm::radians(pitch), 0.0f)) * glm::vec3(0.0f, 0.0f, -1.0f));
	}
}
