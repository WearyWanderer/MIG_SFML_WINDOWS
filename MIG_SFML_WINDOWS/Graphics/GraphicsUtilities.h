#pragma once

#include "../Base/stdafx.h"
#include <SFML/Graphics/Rect.hpp>

namespace gfx
{
	inline void ResetOriginToCenter(sf::Sprite* sprite)
	{
		sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
	};

	inline bool IsOnScreen(sf::Rect<float> spriteRect)
	{


		return true;
	}
}
