#pragma once

#ifdef _WIN32
#include "../Graphics/TextureLibrary.h"
#include "../Base/BaseLoopClass.h"
#include "../Base/stdafx.h"
#elif __APPLE__
#include "..\Graphics\TextureLibrary.h"
#include "..\Base\BaseLoopClass.h"
#include "..\Base\stdafx.h"
#endif

#include "Player.h"

class World : public BaseLoopClass
{
public:
	World() {};
	World(TextureLibrary* worldTextures, sf::RenderWindow* window);
	~World();

	void Init() override;
	void Simulate(float deltaTime) override;
	void Render() override;

	void AddPlayer(int uniquePlayerID, float x, float y, std::pair<sf::Texture, sf::Rect<int>>* animTexture);

private:
	std::vector<Player*> m_players;

	sf::RenderWindow* m_window;
};

