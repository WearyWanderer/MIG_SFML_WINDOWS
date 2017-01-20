#pragma once

enum Scene
{
	MAIN_MENU,
	LOADING_SCREEN,
	GAME_LOOP
};

class StateManager
{
public:
	StateManager();
	~StateManager();

	void SwitchScene(Scene sceneSwitchingTo) { m_currentScene = sceneSwitchingTo; }

	Scene getCurrentScene() { return m_currentScene; }

private:
	Scene m_currentScene = MAIN_MENU; //FOR NOW, THEN SWITCH TO 0 when WE HAVE MENU
};

