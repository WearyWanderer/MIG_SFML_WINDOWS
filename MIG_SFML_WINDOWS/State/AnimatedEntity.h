#pragma once

#ifdef _WIN32
#include "../Base/stdafx.h"
#include "Entity.h"
#elif __APPLE__
#include "..\Base\stdafx.h"
#include "Entity.h"
#endif

class AnimatedEntity : public Entity
{
public:
	AnimatedEntity(float x, float y, sf::Texture* texture, sf::Rect<int> spriteRect, int layer) : Entity(x, y, texture, layer) 
	{
		m_spriteRect = spriteRect;
	};
	~AnimatedEntity();

	sf::Rect<int> getCurrentFrameRect(); //calculates and returns the current frame within the spritesheet. If 0, returns just the spriterect
	int getCurrentFrameNum() { return m_currentFrame; }
	void setNextFrame(float deltaTime);
	void setCurrentFrame(int frame);

	float animSpeed = 1.0f;

private:
	sf::Rect<int> m_spriteRect;
	int			  m_currentFrame = 0;
	float		  m_timer = 0.0f;
};

