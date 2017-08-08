#include "FadeAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>

void FadeAnimation::play(Picture &picture)
{
	startImage.copyTo(picture);

	switch(fadeStyle)
	{
	case FADE_IN:
		picture *= alpha;
		alpha += rate;
		if (alpha >= 1.0f) {
			alpha = 1.0f;
			ended = true;
		}
		break;
	case FADE_OUT:
		picture *= alpha;
		alpha -= rate;
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			ended = true;
		}
		break;
	case FADE_TO:
		addWeighted(startImage, alpha, endImage, (1-alpha), 0.0, picture);
		alpha -= rate;
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			ended = true;
		}
		break;
	default:
		break;
	}
}