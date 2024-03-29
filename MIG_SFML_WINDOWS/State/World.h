#pragma once

#include "../Graphics/TextureLibrary.h"
#include "../Base/BaseLoopClass.h"
#include "../Base/stdafx.h"


#include "Player.h"

class World : public BaseLoopClass
{
public:
	World() {};
	World(sf::RenderWindow* window);
	~World();

	void Init() override {};
	void Init(int playerID, int xPos, int yPos);
	void Simulate(float deltaTime) override;
	void Render() override;

	Player* AddPlayer(int uniquePlayerID, float x, float y, std::pair<sf::Texture, sf::Rect<int>>* animTexture);
	void RemovePlayer(int uniquePlayerID);
	Player GetLocalPlayerCopy() { return *m_localPlayer; }

	//Local user actions within the world
	void MovePlayer(PlayerState state);
	void MovePlayer(int playerID, PlayerState state);
	void MovePlayer(int playerID, float x);

private:
	std::vector<Player*> m_players;
	Player* m_localPlayer; //the local player is the way we will control input for players who're joining a lobby, we'll know our unique players id once the server tells us what it is
};

