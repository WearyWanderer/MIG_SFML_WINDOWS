#pragma once

//STL libraries
#include <string>

//SFML libs
#include <SFML\Graphics.hpp>

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