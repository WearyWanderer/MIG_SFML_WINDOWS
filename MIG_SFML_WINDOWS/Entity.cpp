#include "Entity.h"



Entity::Entity(float x, float y, sf::Texture* texture)
{
	m_sprite.setTexture(*texture);
	m_sprite.setPosition(x, y);
}


Entity::~Entity()
{
}
