#pragma once

#ifdef _WIN32
#include "../Base/stdafx.h"
#elif __APPLE__
#include "../Base\stdafx.h"
#endif

class Entity
{
public:
	Entity() {};
	Entity(float x, float y, sf::Texture* texture, int layer);
	~Entity();

	sf::Sprite m_sprite;
	int m_layer;
};

