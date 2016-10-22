#pragma once

#include "../Base/stdafx.h"

namespace gfx
{
	inline void ResetOriginToCenter(sf::Sprite* sprite)
	{
		sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
	};
}