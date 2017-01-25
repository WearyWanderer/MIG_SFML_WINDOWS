#include "InputManager.h"

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::KeyboardInput()
{
	switch (Application::instance()->StateSystem()->getCurrentScene())
	{
		case MAIN_MENU:
		break;



		case LOADING_SCREEN:
		break;



		case GAME_LOOP:
#pragma region PLAYER MOVEMENT
			bool notMoving = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				Application::instance()->WorldSystem()->MovePlayer(PLAYER_WALKING_LEFT);
				notMoving = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				Application::instance()->WorldSystem()->MovePlayer(PLAYER_WALKING_RIGHT);
				notMoving = false;
			}
			

			if (notMoving)
				Application::instance()->WorldSystem()->MovePlayer(PLAYER_IDLE);
#pragma endregion
#pragma region DEV TOGGLES
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
			{
				unsigned char newFlag = Application::instance()->Client()->GetDevFlags();
				newFlag |= ARTIFICIAL_LATENCY;
				Application::instance()->Client()->SetDevFlags(newFlag);
			}
#pragma endregion
		break;
	}
}
