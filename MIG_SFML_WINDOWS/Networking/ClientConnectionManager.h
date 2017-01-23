#pragma once

#include "../Base/stdafx.h"
#include <SFML\Network.hpp>

class ClientConnectionManager
{
public:
	ClientConnectionManager();
	~ClientConnectionManager();

	void BroadcastLobbySearch();

protected:
	sf::UdpSocket m_socket;
	unsigned int m_portNum;
};

