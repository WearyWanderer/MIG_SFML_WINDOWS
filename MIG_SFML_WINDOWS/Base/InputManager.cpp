#include "InputManager.h"

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::KeyboardInput(StateManager* state, World* world)
{
	switch (state->getCurrentScene())
	{
		case MAIN_MENU:
		break;



		case LOADING_SCREEN:
		break;



		case GAME_LOOP:
			bool notMoving = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				world->MovePlayer(PLAYER_WALKING_LEFT);
				notMoving = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				world->MovePlayer(PLAYER_WALKING_RIGHT);
				notMoving = false;
			}
			

			if (notMoving)
				world->MovePlayer(PLAYER_IDLE);
		break;
	}
}
