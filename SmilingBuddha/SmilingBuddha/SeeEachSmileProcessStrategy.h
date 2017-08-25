/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SEE_EACH_SMILE_PROCESS_STRATEGY_H
#define _SEE_EACH_SMILE_PROCESS_STRATEGY_H

#include "SmileProcessStrategy.h"

class SeeEachSmileProcessStrategy : public SmileProcessStrategy
{
public:
	SeeEachSmileProcessStrategy(SmileObserver * observer);

	~SeeEachSmileProcessStrategy();

	virtual void ProcessSmile(std::shared_ptr<cv::Mat> img, double intensity) override;
	
private:

};

#endif // !_SEE_EACH_SMILE_PROCESS_STRATEGY_H