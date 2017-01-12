#include "World.h"



World::World(TextureLibrary* worldTextures, sf::RenderWindow* window) : BaseLoopClass(window)
{
	m_window = window;

	AddPlayer(0, 0, 0, worldTextures->getAnimatedTexture(0));
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

}

void World::Render()
{
	for (Player* i : m_players)
	{
		m_window->draw(i->m_sprite);
		if (i->getCurrentFrameNum() == 10)
			i->setCurrentFrame(0);
		else
			i->setNextFrame();
	}
}