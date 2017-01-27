#include "World.h"



World::World(sf::RenderWindow* window)
{
	m_window = window;
}


World::~World()
{
	
	//for (Player* i : m_players)
	//	delete(i);
}

Player* World::AddPlayer(int uniquePlayerID, float x, float y, std::pair<sf::Texture, sf::Rect<int>>* animTexture)
{
	m_players.push_back(new Player(uniquePlayerID, x, y, animTexture));
	return m_players.at(m_players.size() - 1);
}

void World::RemovePlayer(int uniquePlayerID)
{
	m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [uniquePlayerID](Player *obj) {return obj->GetPlayerID() == uniquePlayerID; }), m_players.end());
}

void World::Init(int playerID, int xPos, int yPos) 
{
	m_localPlayer = AddPlayer(playerID, xPos, yPos, TexturesSingleton::instance()->m_playerTextures.getAnimatedTexture("p1walkcyclesheet"));
	std::cout << "Successfully connected to lobby, our uniquePlayerID is " << playerID << std::endl;
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

void World::MovePlayer(int playerID, PlayerState state)
{
	for (Player* i : m_players)
	{
		if (i->GetPlayerID() == playerID)
			i->SetPlayerState(state);
	}
}