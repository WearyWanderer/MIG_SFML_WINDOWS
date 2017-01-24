#pragma once

#include "../Base/stdafx.h"
#include "../Base/Application.h"
#include <SFML\Network.hpp>

struct ConnectionInfo
{
public:
	ConnectionInfo() {};
	ConnectionInfo(sf::IpAddress sender, unsigned short portNum) { address = sender; port = portNum; };

	sf::IpAddress address;
	unsigned short port;
};

class ClientConnectionManager
{
public:
	ClientConnectionManager();
	~ClientConnectionManager();

	std::shared_ptr<std::thread> BroadcastLobbySearch();
	void ProcessLobbyResponses();
	bool AttemptConnection(std::string lobbyKey);

protected:
	sf::UdpSocket m_socket;
	unsigned int m_portNum;

	std::shared_ptr<std::thread> m_processThread;

	std::map<std::string, ConnectionInfo> m_detectedLobbies;

	unsigned int m_lobbysFound = 0;
};

