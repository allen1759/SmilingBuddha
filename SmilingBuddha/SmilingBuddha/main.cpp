#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "WebcamSmileVideoProcessor.h"
#include "VideoRenderer.h"
#include "Director.h"

int main()
{
	SmileVideoProcessor *smileVideoProcessor = WebcamSmileVideoProcessor::GetInstance();
	VideoRenderer *videoRenderer = VideoRenderer::GetInstance();
	Director *director = new Director(videoRenderer, smileVideoProcessor);

	videoRenderer->RenderLoop();

	return 0;
}