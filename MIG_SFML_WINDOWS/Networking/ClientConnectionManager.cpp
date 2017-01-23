#include "ClientConnectionManager.h"


ClientConnectionManager::ClientConnectionManager()
{
}


ClientConnectionManager::~ClientConnectionManager()
{
}

void ClientConnectionManager::BroadcastLobbySearch()
{
	char data[100];
	std::size_t sending;
	sf::IpAddress address;
	address = sf::IpAddress::getLocalAddress();

	strcpy_s(data, address.toString().c_str());

	if (m_socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		//debug log an error here
	}
	else
	{
		char datagram[1000];
		size_t recieved;
		sf::IpAddress sender;
		unsigned short port;

		m_socket.send(data, 100, sf::IpAddress::Broadcast, 8080);


		if (m_socket.receive(datagram, 1000, recieved, sender, port) != sf::Socket::Done)
		{
			//error
		}

		std::cout << "Recieved " << datagram << " from " << sender << " on port " << port << std::endl;
	}
}
