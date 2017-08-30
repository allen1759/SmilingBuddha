/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INTERACTION_STATE_H
#define _INTERACTION_STATE_H

class Director;

class InteractionState {
protected:
	InteractionState(Director *director)
	{
	}

public:
	virtual ~InteractionState()
	{
	}

	virtual void Update() = 0;
};


#endif // !_INTERACTION_STATE_H
