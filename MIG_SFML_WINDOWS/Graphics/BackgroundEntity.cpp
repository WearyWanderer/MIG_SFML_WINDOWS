#include "BackgroundEntity.h"

BackgroundEntity::BackgroundEntity(float x, float y, sf::Texture* texture, int layer, float speed, unsigned char textureFlags) : Entity(x, y, texture, layer)
{
	m_speed = speed;
	m_flags = textureFlags;
}


BackgroundEntity::~BackgroundEntity()
{
}
