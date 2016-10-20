#pragma once

#include "Base\stdafx.h"

class Entity
{
public:
	Entity(float x, float y, sf::Texture* texture);
	~Entity();

	sf::Sprite m_sprite;
};

