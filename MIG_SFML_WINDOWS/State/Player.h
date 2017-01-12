#pragma once
#include "AnimatedEntity.h"

/*additional future imp[ notes

-the first player (the host) will always be assigned unique ID 0 but the players who join later will be assigned unique IDs

*/
class Player : public AnimatedEntity
{
public:
	enum PlayerState
	{
		PLAYER_IDLE,
		PLAYER_WALKING_LEFT,
		PLAYER_WALKING_RIGHT,
		PLAYER_CROUCHING,
		PLAYER_JUMPING
	};

	Player(float x, float y, std::pair<sf::Texture, sf::Rect<int>>* animTexture) : AnimatedEntity(x, y, &animTexture->first, animTexture->second, 0) 
	{
		animSpeed = 0.08f;
		m_sprite.setTextureRect(animTexture->second);
		m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height);
	};
	~Player();

	void SetPlayerNextTexture();

private:

	float m_moveSpeed = 2.0f;
	PlayerState m_state = PLAYER_IDLE;
};

