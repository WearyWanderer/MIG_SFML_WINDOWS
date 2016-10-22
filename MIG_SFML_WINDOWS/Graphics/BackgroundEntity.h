#pragma once

#ifdef _WIN32
#include "../Base/stdafx.h"
#include "../Entity.h"
#elif __APPLE__
#include "..\Base\stdafx.h"
#include "..\Entity.h"
#endif

//Define our bit flags
const unsigned char fitToScreen = 0x01; 
const unsigned char tileTexture = 0x02; 
const unsigned char wrapAroundScreen = 0x04; 
const unsigned char onlyMoveWithPlayer = 0x08; //used for parallax textures

class BackgroundEntity : public Entity
{
public:
	BackgroundEntity() {};
	BackgroundEntity(float x, float y, sf::Texture* texture, int layer, float speed, unsigned char textureFlags);
	~BackgroundEntity();

	float m_speed; //the speed the entity moves across the screen
	unsigned char m_flags = 0; //various flags in a bit mask
};
