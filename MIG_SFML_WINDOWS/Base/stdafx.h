#pragma once

//STL libraries
#include <string>
#include <thread>
#include <mutex>

//Debug mem leak logs on Windows
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#elif __APPLE__

#endif

//SFML libs
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <TGUI\TGUI.hpp>

#include "..\Graphics\TexturesSingleton.h"

#define WIDTH 1360
#define HEIGHT 768

#define BUFFSIZE 256

//The layers used for rendering order, as well as for collision objects
enum GameLayers
{
	PLAYERS,
	COLLISIONS,
	BACKGROUND_1,
	BACKGROUND_2,
	BACKGROUND_3,
	BACKGROUND_4
};