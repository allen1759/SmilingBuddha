/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "Kinect1HeadPoseTracker.h"

#include <stdexcept>
#include <algorithm>
#include <cmath>

#include <iostream>


Kinect1HeadPoseTracker::Kinect1HeadPoseTracker()
{
	InitializeColorBuffer();
	InitializeDepthBuffer();
	InitializeSkeletonData();

	trackingThread = NULL;

	isKinectOpened = false;
	isTracking = false;
}

Kinect1HeadPoseTracker::~Kinect1HeadPoseTracker()
{
	StopTracking();

	if (trackingThread) {
		trackingThread->join();
		trackingThread = NULL;
	}

	ReleaseKinectResources();

	if (colorBuffer) {
		colorBuffer->Release();
		colorBuffer = NULL;
	}
	if (depthBuffer) {
		depthBuffer->Release();
		depthBuffer = NULL;
	}
}

void Kinect1HeadPoseTracker::StartTracking()
{
	if (!isTracking) {
		if (trackingThread) {
			trackingThread->join();
			trackingThread = NULL;
		}

		isTracking = true;
		trackingThread = std::make_shared<std::thread>(&Kinect1HeadPoseTracker::Tracking, this);
	}
}

void Kinect1HeadPoseTracker::StopTracking()
{
	isTracking = false;
}

void Kinect1HeadPoseTracker::InitializeColorBuffer()
{
	HRESULT hr = E_UNEXPECTED;

	colorBuffer = NULL;
	colorBuffer = FTCreateImage();
	if (!colorBuffer)
		throw std::runtime_error("Out of memory");

	hr = colorBuffer->Allocate(640, 480, FTIMAGEFORMAT_UINT8_B8G8R8X8);
	if (hr == E_INVALIDARG)
		throw std::runtime_error("IO exception");
	else if (hr == E_OUTOFMEMORY)
		throw std::runtime_error("Out of memory");
}

void Kinect1HeadPoseTracker::InitializeDepthBuffer()
{
	HRESULT hr = E_UNEXPECTED;

	depthBuffer = NULL;
	depthBuffer = FTCreateImage();
	if (!depthBuffer)
		throw std::runtime_error("Out of memory");

	hr = depthBuffer->Allocate(320, 240, FTIMAGEFORMAT_UINT16_D13P3);
	if (hr == E_INVALIDARG)
		throw std::runtime_error("IO exception");
	else if (hr == E_OUTOFMEMORY)
		throw std::runtime_error("Out of memory");
}

void Kinect1HeadPoseTracker::InitializeSkeletonData()
{
	for (int i = 0; i < NUI_SKELETON_COUNT; ++i) {
		neckPoints[i] = FT_VECTOR3D(0.0f, 0.0f, 0.0f);
		headPoints[i] = FT_VECTOR3D(0.0f, 0.0f, 0.0f);
		skeletonTracked[i] = false;
	}
}

void Kinect1HeadPoseTracker::Tracking()
{
	while (isTracking) {
		try {
			OpenKinectSensor();
			InitializeFaceTracker();
			TrackFace();
		}
		catch (std::exception &e) {
			ReleaseKinectResources();
			std::cerr << "Cannot open Kinect." << std::endl;
		}
	}

	ReleaseKinectResources();
}

void Kinect1HeadPoseTracker::TrackFace()
{
	HANDLE events[3] = { colorFrameEvent, depthFrameEvent, skeletonFrameEvent };

	lastTraslation = glm::vec3(0.0f, 0.0f, 0.0f);
	lastRotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	while (isTracking) {
		// Wait frame signal for 100 milisecond.
		int signal = WaitForMultipleObjects(sizeof(events) / sizeof(HANDLE), events, true, 100);

		if (signal == WAIT_TIMEOUT || signal == WAIT_FAILED)
			throw std::runtime_error("Error tracking face.");

		ProcessColorFrame();
		ProcessDepthFrame();
		ProcessSkeletonFrame();
		ProcessFaceResult();
	}
}

void Kinect1HeadPoseTracker::OpenKinectSensor()
{
	HRESULT hr = E_UNEXPECTED;
	hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
	if (FAILED(hr))
		throw std::runtime_error("Error initializing Kinect 1 sensor.");

	colorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	depthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	skeletonFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, colorFrameEvent, &colorStreamHandle);
	if (FAILED(hr))
		throw std::runtime_error("Error initializing Kinect 1 sensor.");

	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_320x240, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE, 2, depthFrameEvent, &depthStreamHandle);
	if (FAILED(hr))
		throw std::runtime_error("Error initializing Kinect 1 sensor.");

	hr = NuiSkeletonTrackingEnable(skeletonFrameEvent, NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE | NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
	if (FAILED(hr))
		throw std::runtime_error("Error initializing Kinect 1 sensor.");

	if (SUCCEEDED(hr))
		isKinectOpened = true;
}

void Kinect1HeadPoseTracker::InitializeFaceTracker()
{
	HRESULT hr = E_UNEXPECTED;

	faceTracker = FTCreateFaceTracker();
	if (!faceTracker)
		throw std::runtime_error("Error initializing Kinect 1 face tracker");

	FT_CAMERA_CONFIG colorCameraConfig = { 640, 480, NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS };
	FT_CAMERA_CONFIG depthCameraConfig = { 320, 240, NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS };

	hr = faceTracker->Initialize(&colorCameraConfig, &depthCameraConfig, NULL, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initializing Kinect 1 face tracker");

	hr = faceTracker->CreateFTResult(&faceTrackingResult);
	if (FAILED(hr) || !faceTrackingResult)
		throw std::runtime_error("Error initializing Kinect 1 face tracker");

	lastTrackSucceeded = false;

	trackingHints = new FT_VECTOR3D[2];
	trackingHints[0] = FT_VECTOR3D(0.0f, 0.0f, 0.0f);
	trackingHints[1] = FT_VECTOR3D(0.0f, 0.0f, 0.0f);
}

void Kinect1HeadPoseTracker::ReleaseKinectResources()
{
	if (isKinectOpened) {
		NuiShutdown();
	}
	isKinectOpened = false;

	if (colorFrameEvent && colorFrameEvent != INVALID_HANDLE_VALUE){
		CloseHandle(colorFrameEvent);
		colorFrameEvent = NULL;
	}
	if (depthFrameEvent && depthFrameEvent != INVALID_HANDLE_VALUE) {
		CloseHandle(depthFrameEvent);
		depthFrameEvent = NULL;
	}
	if (skeletonFrameEvent && skeletonFrameEvent != INVALID_HANDLE_VALUE) {
		CloseHandle(skeletonFrameEvent);
		skeletonFrameEvent = NULL;
	}

	if (faceTracker) {
		faceTracker->Release();
		faceTracker = NULL;
	}

	if (faceTrackingResult) {
		faceTrackingResult->Release();
		faceTrackingResult = NULL;
	}

}


void Kinect1HeadPoseTracker::ProcessColorFrame()
{
	const NUI_IMAGE_FRAME* imageFrame = NULL;

	HRESULT hr = NuiImageStreamGetNextFrame(colorStreamHandle, 0, &imageFrame);
	if (FAILED(hr))
		return;

	INuiFrameTexture* texture = imageFrame->pFrameTexture;
	NUI_LOCKED_RECT lockedRect;
	texture->LockRect(0, &lockedRect, NULL, 0);
	if (lockedRect.Pitch) {
		unsigned int bufferLength = min(colorBuffer->GetBufferSize(), static_cast<unsigned int>(texture->BufferLen()));
		std::copy(lockedRect.pBits, lockedRect.pBits + bufferLength, colorBuffer->GetBuffer());
	}

	hr = NuiImageStreamReleaseFrame(colorStreamHandle, imageFrame);
}

void Kinect1HeadPoseTracker::ProcessDepthFrame()
{
	const NUI_IMAGE_FRAME* imageFrame = NULL;

	HRESULT hr = NuiImageStreamGetNextFrame(depthStreamHandle, 0, &imageFrame);
	if (FAILED(hr))
		return;

	INuiFrameTexture* texture = imageFrame->pFrameTexture;
	NUI_LOCKED_RECT lockedRect;
	texture->LockRect(0, &lockedRect, NULL, 0);
	if (lockedRect.Pitch) {
		unsigned int bufferLength = min(depthBuffer->GetBufferSize(), static_cast<unsigned int>(texture->BufferLen()));
		std::copy(lockedRect.pBits, lockedRect.pBits + bufferLength, depthBuffer->GetBuffer());
	}

	hr = NuiImageStreamReleaseFrame(depthStreamHandle, imageFrame);
}

void Kinect1HeadPoseTracker::ProcessSkeletonFrame()
{
	NUI_SKELETON_FRAME skeletonFrame = { 0 };

	HRESULT hr = NuiSkeletonGetNextFrame(0, &skeletonFrame);
	if (FAILED(hr))
		return;

	for (int i = 0; i < NUI_SKELETON_COUNT; ++i) {
		if (skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED &&
			skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_HEAD] == NUI_SKELETON_POSITION_TRACKED &&
			skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_SHOULDER_CENTER] == NUI_SKELETON_POSITION_TRACKED) {
			skeletonTracked[i] = true;
			
			headPoints[i].x = skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x;
			headPoints[i].y = skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y;
			headPoints[i].z = skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z;
			
			neckPoints[i].x = skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x;
			neckPoints[i].y = skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y;
			neckPoints[i].z = skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z;
		}
		else {
			skeletonTracked[i] = false;
			headPoints[i] = FT_VECTOR3D(0, 0, 0);
			neckPoints[i] = FT_VECTOR3D(0, 0, 0);
		}
	}
}

void Kinect1HeadPoseTracker::ProcessFaceResult()
{
	HRESULT hr = E_FAIL;

	FT_SENSOR_DATA sensorData(colorBuffer, depthBuffer);

	CalculateTrackingHint();

	if (lastTrackSucceeded)
		hr = faceTracker->ContinueTracking(&sensorData, trackingHints, faceTrackingResult);
	else
		hr = faceTracker->StartTracking(&sensorData, NULL, trackingHints, faceTrackingResult);

	lastTrackSucceeded = SUCCEEDED(hr) && SUCCEEDED(faceTrackingResult->GetStatus());
	if (lastTrackSucceeded) {
		float scale;
		float rotationXYZ[3];
		float translationXYZ[3];
		faceTrackingResult->Get3DPose(&scale, rotationXYZ, translationXYZ);

		glm::vec3 translation = glm::vec3(translationXYZ[0], translationXYZ[1], translationXYZ[2]);
		glm::quat rotation = glm::quat(glm::vec3(glm::radians(rotationXYZ[0]), glm::radians(rotationXYZ[1]), glm::radians(rotationXYZ[2])));

		glm::vec3 currentTranslation = glm::mix(translation, lastTraslation, 0.5f);
		lastTraslation = currentTranslation;
		headPose.SetOrigin(currentTranslation);

		glm::quat currentRotation = glm::mix(rotation, lastRotation, 0.5f);
		lastRotation = currentRotation;
		headPose.SetDirection(currentRotation * glm::vec3(0.0f, 0.0f, -1.0f));
	}
	else
		faceTrackingResult->Reset();
}


void Kinect1HeadPoseTracker::CalculateTrackingHint()
{
	int closestSkeletonIndex = -1;
	float smallestDistance = 0;

	if (trackingHints[1].x == 0 && trackingHints[1].y == 0 && trackingHints[1].z == 0) {
		// Get the skeleton closest to the camera
		for (int i = 0; i < NUI_SKELETON_COUNT; ++i) {
			if (skeletonTracked[i] && (closestSkeletonIndex == -1 || headPoints[i].z < smallestDistance)) {
				smallestDistance = headPoints[i].z;
				closestSkeletonIndex = i;
			}
		}
	}
	else{
		// Get the skeleton closest to the previous position
		for (int i = 0; i < NUI_SKELETON_COUNT; i++) {
			if (skeletonTracked[i]) {
				// calculate distance using L1-norm.
				float distance = std::abs(headPoints[i].x - trackingHints[1].x) +
					std::abs(headPoints[i].y - trackingHints[1].y) +
					std::abs(headPoints[i].z - trackingHints[1].z);
				
				if (smallestDistance == 0 || distance < smallestDistance) {
					smallestDistance = distance;
					closestSkeletonIndex = i;
				}
			}
		}
	}

	if (closestSkeletonIndex != -1) {
		trackingHints[0] = neckPoints[closestSkeletonIndex];
		trackingHints[1] = headPoints[closestSkeletonIndex];
	}
}
