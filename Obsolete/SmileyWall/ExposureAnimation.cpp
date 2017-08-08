#include "ExposureAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>

void ExposureAnimation::play(Picture &picture)
{
	startImage.copyTo(picture);
	switch (exposureStyle)
	{
	case pic2exp:
		picture += cv::Scalar(scalar, scalar, scalar);
		scalar += speed;
		if (scalar >= bound)
			ended = true;
		break;
	case exp2pic:
		picture += cv::Scalar(scalar, scalar, scalar);
		scalar -= speed;
		if (scalar <= bound)
			ended = true;
		break;
	default:
		break;
	}
}