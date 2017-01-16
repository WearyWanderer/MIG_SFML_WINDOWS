#pragma once

#include "stdafx.h"
#include <SFML\Window.hpp>
#ifdef _WIN32
#include "../State/StateManager.h"
#include "../State/World.h"
#elif __APPLE__
#endif

class InputManager
{
public:
	InputManager();
	~InputManager();

	//Handles all the actions that we can take via keyboard input
	void KeyboardInput(StateManager* state, World* world);
};

