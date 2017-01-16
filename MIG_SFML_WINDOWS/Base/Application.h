#pragma once

#include "stdafx.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#ifdef _WIN32
	#include "../Graphics/TextureLibrary.h"
	#include "../State/AnimatedEntity.h"
	#include "../Graphics/BackgroundRenderer.h"
	#include "../State/World.h"
	#include "../Base/InputManager.h"
#elif __APPLE__
	#include "..\Graphics\TextureLibrary.h"
	#include "..\State\Entity.h"
	#include "..\Graphics\BackgroundRenderer.h"
	#include "..\State\World.h"
	#include "..\Base\InputManager.h"
#endif


class Application
{
public:
	Application();
	~Application();

	
	void Init(); //Load the various needed resources into memory and containers

	void Update(); //Update and modify anything that we need to based on network packets or messages recieved, stored in a thread safe container

	void Simulate(float deltaTime); //Simulation of the game state, which takes in time

	void Render(); //Render our current state, which can run as fast as the cpu can cope with

	inline bool isRunning() { return m_window.isOpen(); }

protected:
	sf::RenderWindow m_window;

#pragma region Graphics/Rendering Components
	TextureLibrary m_bgTextures;
	TextureLibrary m_playerTextures;



	BackgroundRenderer m_bgRenderer; //could put both these into a BaseLoopClass vector and just loop in the init simulate and renders


	StateManager m_state;
	InputManager m_input;
	World m_mainWorld;
#pragma endregion
};

