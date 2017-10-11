/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _ROI_STATE_H
#define _ROI_STATE_H

#include "InteractionState.h"

#include <opencv2\videoio\videoio.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "Setting.h"
#include "VideoRenderer.h"
#include "Director.h"

// State
#include "PreludeInitialState.h"

class RoiState : public InteractionState
{
public:
	RoiState(Director *director)
		: InteractionState(director)
	{
		videoCapture.open(Setting::GetInstance()->GetCameraIndex());
		videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, Setting::GetInstance()->GetResolutionWidth());
		videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, Setting::GetInstance()->GetResolutionHeight());
	}

	virtual ~RoiState()
	{
		videoCapture.release();
		cv::destroyAllWindows();
	}

	virtual void Update() override
	{
		videoCapture >> frame;
		cv::Rect detectRoi = Setting::GetInstance()->GetDetectRoi();
		cv::rectangle(frame, detectRoi, cv::Scalar(0, 0, 255), 5);
		cv::imshow("Detect ROI", frame);
		cv::waitKey(1);

		// Move the Roi top-left point.
		if (InputManager::GetInstance()->GetKey('a'))
			detectRoi.x--;
		else if (InputManager::GetInstance()->GetKey('d'))
			detectRoi.x++;
		else if (InputManager::GetInstance()->GetKey('w'))
			detectRoi.y--;
		else if (InputManager::GetInstance()->GetKey('s'))
			detectRoi.y++;
		// Adjust the width and height
		else if (InputManager::GetInstance()->GetKey('f'))
			detectRoi.width--;
		else if (InputManager::GetInstance()->GetKey('h'))
			detectRoi.width++;
		else if (InputManager::GetInstance()->GetKey('t'))
			detectRoi.height--;
		else if (InputManager::GetInstance()->GetKey('g'))
			detectRoi.height++;

		Setting::GetInstance()->SetDetectRoi(detectRoi);
	}

	virtual std::string ToString() override
	{
		return "RoiState";
	}

	virtual void OnRoiTrigger() override
	{
		Setting::GetInstance()->SaveDetectRoi();
		director->SetInteractionState(std::make_shared<PreludeInitialState>(director));
	}

private:

	cv::VideoCapture videoCapture;
	cv::Mat frame;
};


#endif // !_ROI_STATE_H
