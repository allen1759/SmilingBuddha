/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTERACTION_STATE_H
#define _INTERACTION_STATE_H

#include <string>
#include <vector>
#include <memory>

#include <opencv2\core\core.hpp>

#include "Event.h"

class Director;

class InteractionState : public std::enable_shared_from_this<InteractionState>
{
protected:
	InteractionState(Director *director);

public:
	virtual ~InteractionState();

	virtual void Update() = 0;

	virtual std::string ToString() = 0;

	virtual void SignalEvent(std::shared_ptr<Event> e);

	virtual void OnSmile();

	virtual void OnRecorded(std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> images);

	//virtual void OnUserDetect();

	//virtual void OnUserLeave();

protected:
	Director *director;
};


#endif // !_INTERACTION_STATE_H
