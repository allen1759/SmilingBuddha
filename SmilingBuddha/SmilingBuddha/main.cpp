#include "VideoPool.h"
#include "VideoRenderer.h"
#include "Director.h"

// Controller
#include "DebugUserDetector.h"
#include "ArduinoUserDetector.h"
#include "DebugSmileVideoProcessor.h"
#include "WebcamSmileVideoProcessor.h"
#include "DebugHeadPoseTracker.h"
#include "Kinect1HeadPoseTracker.h"

//#include "vld.h"

int main()
{
	// Pre-load.
	VideoPool::GetInstance();

	UserDetector *userDetector = DebugUserDetector::GetInstance();
	//UserDetector *userDetector = ArduinoUserDetector::GetInstance();

	//SmileVideoProcessor *smileVideoProcessor = DebugSmileVideoProcessor::GetInstance();
	SmileVideoProcessor *smileVideoProcessor = WebcamSmileVideoProcessor::GetInstance();

	HeadPoseTracker *headPoseTracker = new DebugHeadPoseTracker();
	//HeadPoseTracker *headPoseTracker = new Kinect1HeadPoseTracker();

	VideoRenderer *videoRenderer = VideoRenderer::GetInstance();

	Director *director = new Director(videoRenderer, smileVideoProcessor, headPoseTracker, userDetector);
	
	videoRenderer->RenderLoop();

	return 0;
}