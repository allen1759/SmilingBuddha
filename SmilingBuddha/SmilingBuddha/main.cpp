#include <iostream>
#include "VideoClip.h"
#include "AnimatedVideo.h"
#include "TransitinalVideo.h"

int main()
{
	Video * v1 = new VideoClip(NULL);
	Video * v2 = new VideoClip(NULL);
	v1 = new AnimatedVideo(v1);
	v1 = new AnimatedVideo(v1);
	v2 = new TransitionalVideo(v1, v2);

	v1->GetFrame();
	std::cout << std::endl;
	v2->GetFrame();

	return 0;
}