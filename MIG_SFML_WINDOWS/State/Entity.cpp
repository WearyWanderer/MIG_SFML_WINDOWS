#include "Entity.h"



Entity::Entity(float x, float y, sf::Texture* texture, int layer)
{
	m_sprite.setTexture(*texture);
	m_sprite.setPosition(x, y);
	m_layer = layer;
}


Entity::~Entity()
{
}
