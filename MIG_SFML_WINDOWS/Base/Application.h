#pragma once

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#ifdef _WIN32
	#include "stdafx.h"
	#include "../State/AnimatedEntity.h"
	#include "../Graphics/BackgroundRenderer.h"
	#include "../State/World.h"
	#include "../Base/InputManager.h"
	#include "../State/MainMenu.h"
	#include "../Networking/ClientConnectionManager.h"
#elif __APPLE__
	#include "..\Graphics\TextureLibrary.h"
	#include "..\State\Entity.h"
	#include "..\Graphics\BackgroundRenderer.h"
	#include "..\State\World.h"
	#include "..\Base\InputManager.h"
#endif

class MainMenu;
class ClientConnectionManager;

class Application
{
public:
	static Application* instance()
	{
		static Application m_instance;

		return &m_instance;
	}
	Application();
	~Application();

	Application(Application const&) = delete;
	void operator=(Application const&) = delete;
	
	void Init(); //Load the various needed resources into memory and containers

	void Update(); //Update and modify anything that we need to based on network packets or messages recieved, stored in a thread safe container

	void Simulate(float deltaTime); //Simulation of the game state, which takes in time

	void Render(); //Render our current state, which can run as fast as the cpu can cope with

	inline bool isRunning() { return m_window.isOpen(); }

	inline World* WorldSystem() { return m_mainWorld.get(); }
	inline StateManager* StateSystem() { return m_state.get(); }
	inline ClientConnectionManager* Client() { return m_client.get(); }
	inline MainMenu* Menu() { return m_menu.get(); }

protected:
	sf::RenderWindow m_window;

#pragma region 

	std::shared_ptr<BackgroundRenderer> m_bgRenderer; //could put both these into a BaseLoopClass vector and just loop in the init simulate and renders

	std::shared_ptr<StateManager> m_state;
	std::shared_ptr<InputManager> m_input;
	tgui::Gui m_gui;

	std::shared_ptr<MainMenu> m_menu;
	std::shared_ptr<World> m_mainWorld;
#pragma endregion

#pragma region TEMP NETWORKING
	std::shared_ptr<ClientConnectionManager> m_client;
#pragma endregion
};

