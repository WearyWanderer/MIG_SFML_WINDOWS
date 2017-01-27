#include "ClientConnectionManager.h"


ClientConnectionManager::ClientConnectionManager()
{
}


ClientConnectionManager::~ClientConnectionManager()
{
	if (m_commSendThread != nullptr)
	{
		m_tcpCommSocket.send("disconnect;", (size_t)sizeof("disconnect;"));
		m_tcpCommSocket.disconnect();
		m_udpCommSocket.unbind();
	}
}

void ClientConnectionManager::StartClientCommunication()
{
	//start the thread which will send player data via the udp connection
	m_commSendThread = std::make_shared<std::thread>(&ClientConnectionManager::SendData, this);
	m_commSendThread->detach();

	//start the thread which will recieve player data via the UDP and TCP connections
	m_commRecieveThread = std::make_shared<std::thread>(&ClientConnectionManager::RecieveData, this);
	m_commRecieveThread->detach();
}

void ClientConnectionManager::SendData()
{
	Player localPlayerCopy = Application::instance()->WorldSystem()->GetLocalPlayerCopy();
	while (Application::instance()->StateSystem()->getCurrentScene() == GAME_LOOP)
	{
		//send our positional data in a packet here with the 'position' and 'playerID'
		SendPlayerPosition(&localPlayerCopy);



	}
}

//curently does not catch paritally complete or partially sent messages, need a nice packet structure to do this instead
void ClientConnectionManager::RecieveData()
{
	while (Application::instance()->StateSystem()->getCurrentScene() == GAME_LOOP)
	{
		char tcpData[BUFFSIZE];
		size_t recieved;
		m_tcpCommSocket.receive(tcpData, BUFFSIZE, recieved);

		std::string recievedMsg(tcpData, recieved);
		std::cout << recievedMsg << std::endl;

		std::string initMsg(tcpData, recieved);
		std::string delimiter(";");
		std::list<std::string> tokens = split(initMsg, ";");

		while (!tokens.empty())
		{
			if (tokens.front() == "addplayer")
			{
				tokens.pop_front();
				int playerID = std::stoi(tokens.front());
				tokens.pop_front();
				Application::instance()->WorldSystem()->AddPlayer(playerID, std::stof(tokens.front()), HEIGHT, TexturesSingleton::instance()->m_playerTextures.getAnimatedTexture("p1walkcyclesheet"));
				tokens.pop_front();
			}
			else if (tokens.front() == "disconnected")
			{
				tokens.pop_front();
				Application::instance()->WorldSystem()->RemovePlayer(std::stoi(tokens.front()));
				tokens.pop_front();
			}
		}

	}
}

void ClientConnectionManager::SendPlayerPosition(Player* playerCopy)
{
	if (playerCopy->GetPlayerState() != PLAYER_IDLE)
	{
		if ((m_flags & ARTIFICIAL_LATENCY) == ARTIFICIAL_LATENCY)
		{
			//send pos data at a delay
		}
	}
}

std::shared_ptr<std::thread> ClientConnectionManager::BroadcastLobbySearch()
{
	if (m_broadcastSocket.getLocalPort() == sf::Socket::NotReady) //if we still need to set up the socket
	{
		if (m_broadcastSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
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
	
	m_broadcastSocket.send(data, 100, sf::IpAddress::Broadcast, 8080);

	//start the thread which listens for responses and processes them as lobby listings here
	m_broadcastThread = std::make_shared<std::thread>(&ClientConnectionManager::ProcessLobbyResponses, this);
	m_broadcastThread->detach();

	return m_broadcastThread;
}

void ClientConnectionManager::ProcessLobbyResponses()
{
	m_broadcastSocket.setBlocking(false);

	while(Application::instance()->Menu()->GetActiveMenu() == LOBBY_SEARCH)
	{
		char datagram[BUFFSIZE];
		size_t recieved = 0;
		sf::IpAddress sender;
		unsigned short port;
		
		if (m_broadcastSocket.receive(datagram, 1000, recieved, sender, port) != sf::Socket::Done)
		{
			//error
		}
#ifdef DEBUG
		std::cout << "Recieved " << recieved << " bytes from " << sender << " on port " << port << std::endl;
#endif // DEBUG

		if (!(sender.toInteger() == 0 && port == 0))
		{
			std::string recievedMsg(datagram, recieved);

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

	sf::Socket::Status status = m_tcpCommSocket.connect(serverToContact.address, serverToContact.port);

	if (status != sf::Socket::Done)
	{
		// error...
	}

	char registerPacket[MAXBUFFSIZE];
	size_t recieved;
	if (m_tcpCommSocket.receive(registerPacket, MAXBUFFSIZE, recieved) != sf::Socket::Done)
	{

	}

	std::string initMsg(registerPacket, recieved);
	std::string delimiter(";");
	std::string token;
	token = initMsg.substr(0, initMsg.find(delimiter));
	initMsg.erase(0, initMsg.find(delimiter) + delimiter.length());

	//first one is uniqueplayerid
	unsigned int playerID = std::stoul(token);
	float xPos = std::stof(initMsg);
	Application::instance()->WorldSystem()->Init(playerID, xPos, HEIGHT);

	Application::instance()->StateSystem()->SwitchScene(GAME_LOOP);
	StartClientCommunication();

	return true;
}

bool ClientConnectionManager::AttemptLocalConnection()
{
	sf::Socket::Status status = m_tcpCommSocket.connect(m_tcpCommSocket.getRemoteAddress(), 8080);

	if (status != sf::Socket::Done)
	{
		// error...
	}

	char registerPacket[MAXBUFFSIZE];
	size_t recieved;
	if (m_tcpCommSocket.receive(registerPacket, MAXBUFFSIZE, recieved) != sf::Socket::Done)
	{

	}

	std::string initMsg(registerPacket, recieved);
	std::string delimiter(";");
	std::string token;
	token = initMsg.substr(0, initMsg.find(delimiter));
	initMsg.erase(0, initMsg.find(delimiter) + delimiter.length());

	//first one is uniqueplayerid
	unsigned int playerID = std::stoul(token);
	float xPos = std::stof(initMsg);
	Application::instance()->WorldSystem()->Init(playerID, xPos, HEIGHT);

	Application::instance()->StateSystem()->SwitchScene(GAME_LOOP);
	StartClientCommunication();

	return true;
}

bool ClientConnectionManager::LaunchServerApplication(std::string lobbyName, std::string password)
{
	std::string execPath = "\"" + Application::instance()->GetAppPath() + "MIG_SFML_SERVER.exe\"";
	//need to add quotations at either side here
	execPath.append(" \"");
	execPath.append(lobbyName);
	execPath.append("\" \"");
	execPath.append(password);
	execPath.append("\"");

	std::cout << execPath << std::endl;

#ifdef _WIN32
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(NULL,   // the path
		LPWSTR(Application::s2ws(execPath).c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,              // Create as a seperate process
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	//system(execPath.c_str());
#elif __APPLE__
	pid_t processId;

	if ((processId = fork()) == 0) 
	{
		char app[] = "DETERMINED PATH NEEDED HERE ANDY";
		char * const argv[] = { app, lobbyName, password };
		if (execv(app, argv) < 0) 
		{
			perror("execv error");
		}
	}
	else if (processId < 0) 
	{
		perror("fork error");
		return false;
	}
#endif

	return true;
}
