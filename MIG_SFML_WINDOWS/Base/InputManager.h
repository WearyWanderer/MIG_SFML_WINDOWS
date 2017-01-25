#pragma once

#include "stdafx.h"
#include <SFML\Window.hpp>
#ifdef _WIN32
#include "Application.h"
#elif __APPLE__
#endif

class InputManager
{
public:
	InputManager();
	~InputManager();

	//Handles all the actions that we can take via keyboard input
	void KeyboardInput();
};

