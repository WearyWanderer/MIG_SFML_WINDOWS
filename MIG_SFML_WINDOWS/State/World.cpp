#include "World.h"



World::World(TextureLibrary* worldTextures, sf::RenderWindow* window) : BaseLoopClass(window)
{
	m_window = window;

	AddPlayer(0, 300, 400, worldTextures->getAnimatedTexture("p1walkcyclesheet"));
	//TEMP
	m_localPlayer = m_players[0];
}


World::~World()
{
}

void World::AddPlayer(int uniquePlayerID, float x, float y, std::pair<sf::Texture, sf::Rect<int>>* animTexture)
{
	m_players.push_back(new Player(x, y, animTexture));
}

void World::Init() 
{

}

void World::Simulate(float deltaTime)
{
	for (Player* i : m_players)
	{
		PlayerState state = i->GetPlayerState();
		if (state == PLAYER_WALKING_LEFT || state == PLAYER_WALKING_RIGHT)
		{
			if (i->getCurrentFrameNum() == 10)
				i->setCurrentFrame(0);
			else
				i->setNextFrame(deltaTime);
		}
		else
			i->setCurrentFrame(0);

		switch (state)
		{
		case PLAYER_WALKING_LEFT:
			i->m_sprite.move(-(i->moveSpeed * deltaTime), 0.0f);
			break;

		case PLAYER_WALKING_RIGHT:
			i->m_sprite.move(i->moveSpeed * deltaTime, 0.0f);
			break;

		case PLAYER_IDLE:
		default:
			break;
		}
	}
}

void World::Render()
{
	for (Player* i : m_players)
	{
		m_window->draw(i->m_sprite);
	}
}

void World::MovePlayer(PlayerState state)
{
	if (m_localPlayer != nullptr /* && notGoingToCollide(some stuff here)*/)
		m_localPlayer->SetPlayerState(state);
}