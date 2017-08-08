/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SEE_EACH_SMILE_PROCESS_H
#define _SEE_EACH_SMILE_PROCESS_H

#include "SmileProcess.h"

class SeeEachSmileProcess : public SmileProcess
{
public:
	SeeEachSmileProcess(SmileObserver * observer);
	~SeeEachSmileProcess();

	virtual void InitializeProcess() override;

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;
	
private:

};

#endif // !_SEE_EACH_SMILE_PROCESS_H