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
	key = '\0';
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

char InputManager::GetKey()
{
	return key;
}

void InputManager::ResetKey()
{
	key = '\0';
}

inline void InputManager::WaitKey()
{
	while (true) {
		if (kbhit())
			key = std::tolower(getch());
		else
			key = '\0';
	}
}
