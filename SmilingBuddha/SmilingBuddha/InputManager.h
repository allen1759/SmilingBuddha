/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <thread>
#include <mutex>
#include <conio.h>

class InputManager
{
private:
	InputManager();

public:
	~InputManager();

	static InputManager *GetInstance();

	bool GetKey(char key);

private:

	void WaitKey();

	static InputManager *instance;
	bool keyTable[128];
	std::mutex keyMutax;
	std::thread inputManagerThread;
};


#endif // !_INPUT_MANAGER_H