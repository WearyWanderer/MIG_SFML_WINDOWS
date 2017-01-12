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
		
		break;
	case PLAYER_WALKING_RIGHT:

		break;
	}


}
