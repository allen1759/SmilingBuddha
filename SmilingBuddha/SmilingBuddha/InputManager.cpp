/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "InputManager.h"

#include <cctype>

#include <iostream>

InputManager *InputManager::instance = NULL;

inline InputManager::InputManager()
{
	for (int i = 0; i < 128; ++i)
		keyTable[i] = false;

	inputManagerThread = std::thread(&InputManager::WaitKey, this);
}

inline InputManager::~InputManager()
{
}

InputManager * InputManager::GetInstance()
{
	if (instance == NULL)
		instance = new InputManager();

	return instance;
}

bool InputManager::GetKey(char key)
{
	if (key >= 128 || key < 0)
		return false;

	keyMutax.lock();
	bool ret = keyTable[key];
	keyTable[key] = false;
	keyMutax.unlock();

	return ret;
}

void InputManager::WaitKey()
{
	while (true) {
		if (kbhit()) {
			char key = std::tolower(getch());
			keyMutax.lock();
			if (key < 128 && key >= 0)
				keyTable[key] = true;
			keyMutax.unlock();
		}
		//if (key != '\0')
		//	std::cout << "current key = " << key << std::endl;
	}
}
