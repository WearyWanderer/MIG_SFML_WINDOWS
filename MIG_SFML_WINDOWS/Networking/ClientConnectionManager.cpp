#include "ClientConnectionManager.h"


ClientConnectionManager::ClientConnectionManager()
{
}


ClientConnectionManager::~ClientConnectionManager()
{
}

std::shared_ptr<std::thread> ClientConnectionManager::BroadcastLobbySearch()
{
	if (m_socket.getLocalPort() == sf::Socket::NotReady) //if we still need to set up the socket
	{
		if (m_socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
		{
			//debug log error here
		}
	}
	else
	{
		//we've already set up in the past and we should stop our thread that has been searching so it can be restarted once we have new info
		m_lobbysFound = 0;
		m_detectedLobbies.clear();
	}

	char data[100];
	std::size_t sending;
	sf::IpAddress address;
	address = sf::IpAddress::getLocalAddress();

	strcpy_s(data, address.toString().c_str());
	
	m_socket.send(data, 100, sf::IpAddress::Broadcast, 8080);

	//start the thread which listens for responses and processes them as lobby listings here
	m_processThread = std::make_shared<std::thread>(&ClientConnectionManager::ProcessLobbyResponses, this);
	m_processThread->detach();

	return m_processThread;
}

void ClientConnectionManager::ProcessLobbyResponses()
{
	m_socket.setBlocking(false);

	while(Application::instance()->Menu()->GetActiveMenu() == LOBBY_SEARCH)
	{
		char datagram[BUFFSIZE];
		size_t recieved = 0;
		sf::IpAddress sender;
		unsigned short port;
		
		if (m_socket.receive(datagram, 1000, recieved, sender, port) != sf::Socket::Done)
		{
			//error
		}
#ifdef DEBUG
		std::cout << "Recieved " << recieved << " bytes from " << sender << " on port " << port << std::endl;
#endif // DEBUG

		if (!(sender.toInteger() == 0 && port == 0))
		{
			std::string recievedMsg = datagram;

			m_lobbysFound++;
			m_detectedLobbies.insert(std::pair<std::string, ConnectionInfo>(recievedMsg, ConnectionInfo(sender, port)));

			Application::instance()->Menu()->lock.lock();
			Application::instance()->Menu()->AddLobbyListing(recievedMsg, m_lobbysFound);
			Application::instance()->Menu()->lock.unlock();
		}
	}
}

bool ClientConnectionManager::AttemptConnection(std::string lobbyKey)
{
	ConnectionInfo serverToContact;
	if (m_detectedLobbies.find(lobbyKey) != m_detectedLobbies.end())
		serverToContact = m_detectedLobbies.find(lobbyKey)->second;
	else
		std::cerr << "ERROR: This lobby doesn't exist!" << std::endl;

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(serverToContact.address, serverToContact.port);

	if (status != sf::Socket::Done)
	{
		// error...
	}

	char data[15] = "requestspawn";
	if (socket.send(data, 15) != sf::Socket::Done)
	{
		// error...
	}

	return true;
}
