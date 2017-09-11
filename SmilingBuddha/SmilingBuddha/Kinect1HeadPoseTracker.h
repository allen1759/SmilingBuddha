/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _KINECT1_HEAD_POSE_TRACKER_H
#define _KINECT1_HEAD_POSE_TRACKER_H

#include "HeadPoseTracker.h"

// STL
#include <thread>
#include <memory>
// Kinect
#include <windows.h>
#include <NuiApi.h>
#include <FaceTrackLib.h>
// GLM
#include<glm/gtc/quaternion.hpp>

class Kinect1HeadPoseTracker : public HeadPoseTracker {
public:
	Kinect1HeadPoseTracker();
	
	~Kinect1HeadPoseTracker();

	virtual void StartTracking();

	virtual void StopTracking();

private:
	void InitializeColorBuffer();

	void InitializeDepthBuffer();

	void InitializeSkeletonData();

	void OpenKinectSensor();

	void InitializeFaceTracker();

	void ReleaseKinectResources();

	void Tracking();

	void TrackFace();

	void ProcessColorFrame();

	void ProcessDepthFrame();

	void ProcessSkeletonFrame();

	void ProcessFaceResult();
	
	void CalculateTrackingHint();
	
	std::shared_ptr<std::thread> trackingThread;
	HANDLE colorFrameEvent;
	HANDLE depthFrameEvent;
	HANDLE skeletonFrameEvent;

	bool isTracking;
	bool isKinectOpened;
	bool isFaceTrackerInitialized;

	HANDLE colorStreamHandle;
	IFTImage* colorBuffer;

	HANDLE depthStreamHandle;
	IFTImage* depthBuffer;
	
	FT_VECTOR3D neckPoints[NUI_SKELETON_COUNT];
	FT_VECTOR3D headPoints[NUI_SKELETON_COUNT];
	bool skeletonTracked[NUI_SKELETON_COUNT];

	IFTFaceTracker* faceTracker;
	IFTResult* faceTrackingResult;
	bool lastTrackSucceeded;
	FT_VECTOR3D *trackingHints;

	glm::vec3 lastTraslation;
	glm::quat lastRotation;
};

#endif