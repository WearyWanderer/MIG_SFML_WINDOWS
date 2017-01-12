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
		sf::Rect<int> newTextRect;
		int numSpritesPerRow = floor(m_sprite.getTexture()->getSize().x / m_spriteRect.width);
		int currRowNum = (m_currentFrame + 1) % numSpritesPerRow;

		return m_spriteRect;
	}
	//TODO - else calculate the spritesheet frame new rect
}

void AnimatedEntity::setNextFrame() 
{ 
	m_currentFrame++; 
	m_sprite.setTextureRect(getCurrentFrameRect());
};

void AnimatedEntity::setCurrentFrame(int frame)
{
	m_currentFrame = 0;
	m_sprite.setTextureRect(getCurrentFrameRect());
};
