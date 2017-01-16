#include "Player.h"

Player::~Player()
{
}

void Player::SetPlayerNextTexture()
{
	setCurrentFrame(0);
	//switch case for different states enum
	switch (m_state)
	{
	case PLAYER_IDLE:

		break;
	case PLAYER_CROUCHING:
		
		break;
	case PLAYER_WALKING_LEFT:
		m_sprite.setScale(-1.0f, 1.0f);
		break;
	case PLAYER_WALKING_RIGHT:
		m_sprite.setScale(1.0f, 1.0f);
		break;
	}


}

void Player::SetPlayerState(PlayerState state) 
{
	if (state != m_state)
	{
		m_state = state;
		SetPlayerNextTexture();
	}
}
