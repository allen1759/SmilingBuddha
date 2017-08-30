#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "VideoRenderer.h"
#include "Director.h"

int main()
{
	VideoRenderer *videoRenderer = VideoRenderer::GetInstance();
	Director *director = new Director(videoRenderer);

	videoRenderer->RenderLoop();

	return 0;
}