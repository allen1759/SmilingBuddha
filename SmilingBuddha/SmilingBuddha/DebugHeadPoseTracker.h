/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _DEBUG_HEAD_POSE_TRACKER_H
#define _DEBUG_HEAD_POSE_TRACKER_H

#include "HeadPoseTracker.h"

#include <memory>
#include <thread>

class DebugHeadPoseTracker : public HeadPoseTracker
{
public:
	DebugHeadPoseTracker();

	~DebugHeadPoseTracker();

	virtual void StartTracking();

	virtual void StopTracking();

private:
	void Tracking();

	std::shared_ptr<std::thread> trackingThread;
	bool isTracking;

	float yaw;
	float pitch;
};

#endif // !_DEBUG_HEAD_POSE_TRACKER
