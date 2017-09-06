#include "DebugHeadPoseTracker.h"

#include <iostream>
#include <conio.h>

#include<glm/gtc/quaternion.hpp>

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
		char key;
		if (kbhit()) {
			key = getch();
			if (key == 'a' || key == 'A')
				pitch++;
			else if (key == 'w' || key == 'W')
				yaw++;
			else if (key == 'd' || key == 'D')
				pitch--;
			else if (key == 's' || key == 'S')
				yaw--;
		}

		headPose.SetDirection(glm::quat(glm::vec3(glm::radians(yaw), glm::radians(pitch), 0.0f)) * glm::vec3(0.0f, 0.0f, -1.0f));
	}
}
