#pragma once

#include "stdafx.h"
#include <SFML/Window.hpp>
#include "Application.h"


class InputManager
{
public:
	InputManager();
	~InputManager();

	//Handles all the actions that we can take via keyboard input
	void KeyboardInput();
};

