#pragma once

#include "../Base/stdafx.h"
#include "../Base/Application.h"
#include <SFML\Network.hpp>

#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#endif

//Define our bit flags
const unsigned char ARTIFICIAL_LATENCY = 0x01;
const unsigned char SIMULATE_HACKING = 0x02;
const unsigned char ARTIFICIAL_TIMEOUT = 0x04;

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

#pragma region Thread Starters
	std::shared_ptr<std::thread> BroadcastLobbySearch();
	std::shared_ptr<std::thread> StartSendingPlayerData();
#pragma endregion

	void SendPlayerPosition();
	void ProcessLobbyResponses();
	bool AttemptConnection(std::string lobbyKey);
	bool AttemptLocalConnection();

	bool LaunchServerApplication(std::string lobbyName, std::string password);

	inline void SetDevFlags(unsigned char flags) { m_flags = flags; }
	inline unsigned char GetDevFlags() { return m_flags; }

protected:
	sf::UdpSocket m_broadcastSocket;
	sf::UdpSocket m_udpCommSocket;
	sf::TcpSocket m_tcpCommSocket;
	sf::TcpListener m_tcpListener;
	
	unsigned int m_portNum;

	std::shared_ptr<std::thread> m_broadcastThread;
	std::shared_ptr<std::thread> m_commThread;

	std::map<std::string, ConnectionInfo> m_detectedLobbies;

	unsigned int m_lobbysFound = 0;

#pragma region DEV FLAGS
	unsigned char m_flags = 0; //various flags in a bit mask
#pragma endregion
};

