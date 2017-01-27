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

const size_t MAXBUFFSIZE = 512;

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
	void StartClientCommunication();
#pragma endregion

	void SendTcpData();
	void RecieveTcpData();
	void SendUdpData();
	void RecieveUdpData();
	void SendPlayerPosition(Player* playerCopy);
	void ProcessLobbyResponses();
	bool AttemptConnection(std::string lobbyKey);
	bool AttemptLocalConnection();

	bool LaunchServerApplication(std::string lobbyName, std::string password);

	inline void SetDevFlags(unsigned char flags) { m_flags = flags; }
	inline unsigned char GetDevFlags() { return m_flags; }

	inline std::list<std::string> split(const std::string& str, const std::string& delim)
	{
		std::list<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos) pos = str.length();
			std::string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	}

protected:
	sf::UdpSocket m_broadcastSocket;
	sf::UdpSocket m_udpCommSocket;
	sf::TcpSocket m_tcpCommSocket;
	
	unsigned int m_portNum; //for udp

	std::shared_ptr<std::thread> m_broadcastThread = nullptr;
	std::shared_ptr<std::thread> m_commUdpSendThread = nullptr;
	std::shared_ptr<std::thread> m_commUdpRecieveThread = nullptr;
	std::shared_ptr<std::thread> m_commTcpSendThread = nullptr;
	std::shared_ptr<std::thread> m_commTcpRecieveThread = nullptr;
	
	std::map<std::string, ConnectionInfo> m_detectedLobbies;

	unsigned int m_lobbysFound = 0;

#pragma region DEV FLAGS
	unsigned char m_flags = 0; //various flags in a bit mask
#pragma endregion
};

