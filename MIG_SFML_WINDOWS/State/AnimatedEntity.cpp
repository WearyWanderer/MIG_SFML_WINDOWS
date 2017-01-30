#include "AnimatedEntity.h"

AnimatedEntity::~AnimatedEntity()
{
}

sf::Rect<int> AnimatedEntity::getCurrentFrameRect()
{
	if (m_currentFrame == 0)
		return m_spriteRect;
	else
	{
		sf::Rect<int> newTextRect = m_spriteRect;
        int numSpritesPerRow = floor(m_sprite.getTexture()->getSize().x / m_spriteRect.width);
		int currRowNum = m_currentFrame / numSpritesPerRow;
		int currColNum = m_currentFrame % numSpritesPerRow;

		newTextRect.top = currRowNum * m_spriteRect.height;
		newTextRect.height = m_spriteRect.height;
		newTextRect.left = currColNum * m_spriteRect.width;
		newTextRect.width = m_spriteRect.width;

		return newTextRect;
	}
	//TODO - else calculate the spritesheet frame new rect
}

void AnimatedEntity::setNextFrame(float deltaTime) 
{ 
	m_timer += deltaTime;
	if (m_timer > animSpeed)
	{
		m_currentFrame++;
		m_sprite.setTextureRect(getCurrentFrameRect());
		m_timer = 0.0f;
	}
};

void AnimatedEntity::setCurrentFrame(int frame)
{
	m_currentFrame = 0;
	m_sprite.setTextureRect(getCurrentFrameRect());
};
