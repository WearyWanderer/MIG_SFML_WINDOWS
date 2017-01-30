#pragma once

#include "../Base/stdafx.h"


class Entity
{
public:
	Entity() {};
	Entity(float x, float y, sf::Texture* texture, int layer);
	~Entity();

	sf::Sprite m_sprite;
	int m_layer;
};

