/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <thread>
#include <conio.h>

class InputManager
{
private:
	InputManager();

public:
	~InputManager();

	static InputManager *GetInstance();

	char GetKey();

	void ResetKey();

private:

	void WaitKey();

	static InputManager *instance;
	char key;
	std::thread inputManagerThread;
};


#endif // !_INPUT_MANAGER_H