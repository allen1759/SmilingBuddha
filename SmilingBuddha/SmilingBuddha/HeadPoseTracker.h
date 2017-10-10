/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _HEAD_POSE_TRACKER_H
#define _HEAD_POSE_TRACKER_H

#include "Ray.h"

class HeadPoseTracker {

public:
	virtual ~HeadPoseTracker()
	{
	}

	virtual void StartTracking() = 0;

	virtual void StopTracking() = 0;
	
	Ray GetHeadPose()
	{
		return headPose;
	}

protected:
	Ray headPose;
};

#endif