#include "ClientConnectionManager.h"


ClientConnectionManager::ClientConnectionManager()
{
}


ClientConnectionManager::~ClientConnectionManager()
{
	if (m_commTcpSendThread != nullptr)
	{
		m_tcpCommSocket.send("dp;", (size_t)sizeof("dp;"));
		m_tcpCommSocket.disconnect();
		m_udpCommSocket.unbind();
	}
}

void ClientConnectionManager::SendTcpShutdown()
{
	m_tcpCommSocket.setBlocking(true);
	std::string msg = "sd;ep;";
	m_tcpCommSocket.send(msg.c_str(), msg.length());
}

void ClientConnectionManager::StartClientCommunication()
{
	//if (Application::instance()->IsHost())
	//{
	//	std::string msg = "ah;ep;";
	//	m_tcpCommSocket.send(msg.c_str(), msg.length());
	//}

	//start the thread which will send player data via the udp connection
	m_commUdpSendThread = std::make_shared<std::thread>(&ClientConnectionManager::SendUdpData, this);
	m_commUdpSendThread->detach();

	//start the thread which will recieve player data via the UDP and TCP connections
	m_commUdpRecieveThread = std::make_shared<std::thread>(&ClientConnectionManager::RecieveUdpData, this);
	m_commUdpRecieveThread->detach();

	//start the thread which will send player data via the udp connection
	m_commTcpSendThread = std::make_shared<std::thread>(&ClientConnectionManager::SendTcpData, this);
	m_commTcpSendThread->detach();

	//start the thread which will recieve player data via the UDP and TCP connections
	m_commTcpRecieveThread = std::make_shared<std::thread>(&ClientConnectionManager::RecieveTcpData, this);
	m_commTcpRecieveThread->detach();
}

void ClientConnectionManager::SendTcpData()
{
	while (Application::instance()->StateSystem()->getCurrentScene() == GAME_LOOP && !Application::instance()->closing)
	{

	}
}

void ClientConnectionManager::SendUdpData()
{
	PlayerState currentSentServerState = PLAYER_IDLE;
	while (Application::instance()->StateSystem()->getCurrentScene() == GAME_LOOP && !Application::instance()->closing)
	{
		Player localPlayerCopy = Application::instance()->WorldSystem()->GetLocalPlayerCopy();
		//send our positional data in a packet here with the 'position' and 'playerID'
		if (localPlayerCopy.GetPlayerState() != currentSentServerState)
		{
			SendPlayerPosition(&localPlayerCopy);
			currentSentServerState = localPlayerCopy.GetPlayerState();
		}
	}
}

//curently does not catch paritally complete or partially sent messages, need a nice packet structure to do this instead
void ClientConnectionManager::RecieveTcpData()
{
	while (Application::instance()->StateSystem()->getCurrentScene() == GAME_LOOP && !Application::instance()->closing)
	{
		char tcpData[BUFFSIZE];
		size_t recieved;
		m_tcpCommSocket.receive(tcpData, BUFFSIZE, recieved);

		std::string recievedMsg(tcpData, recieved);
		std::cout << recievedMsg << std::endl;

		std::list<std::string> tokens = split(recievedMsg, ";");

		while (!tokens.empty())
		{
			if (tokens.front() == "ap")
			{
				tokens.pop_front();
				int playerID = std::stoi(tokens.front());
				tokens.pop_front();
				Application::instance()->WorldSystem()->AddPlayer(playerID, std::stof(tokens.front()), HEIGHT, TexturesSingleton::instance()->m_playerTextures.getAnimatedTexture("p1walkcyclesheet"));
				tokens.pop_front();
			}
			else if (tokens.front() == "dp")
			{
				tokens.pop_front();
				Application::instance()->WorldSystem()->RemovePlayer(std::stoi(tokens.front()));
				tokens.pop_front();
			}
			else
				tokens.pop_front();
		}
	}
}

void ClientConnectionManager::RecieveUdpData()
{
	while (Application::instance()->StateSystem()->getCurrentScene() == GAME_LOOP && !Application::instance()->closing)
	{
		char udpData[BUFFSIZE];
		size_t recieved;
		sf::IpAddress sender;
		unsigned short port;

		m_udpCommSocket.receive(udpData, BUFFSIZE, recieved, sender, port);

		std::string recievedMsg = std::string(udpData, recieved);
		std::cout << recievedMsg << std::endl;

		std::list<std::string> tokens = split(recievedMsg, ";");

		while (!tokens.empty())
		{
			if (tokens.front() == "mp")
			{
				tokens.pop_front();
				int playerID = std::stoi(tokens.front());
				tokens.pop_front();
				int playerState = std::stoi(tokens.front());
				tokens.pop_front();
				float xPos = std::stof(tokens.front());
				Application::instance()->WorldSystem()->MovePlayer(playerID, xPos);
				Application::instance()->WorldSystem()->MovePlayer(playerID, (PlayerState)playerState);
				tokens.pop_front();
			}
			else
				tokens.pop_front();
		}
	}
}

void ClientConnectionManager::SendPlayerPosition(Player* playerCopy)
{
		if ((m_flags & ARTIFICIAL_LATENCY) == ARTIFICIAL_LATENCY)
		{
			//send pos data at a delay instead by doing some sort of sleep here
			
		}

		std::string playPosMsg = "mp;" + std::to_string((int)playerCopy->GetPlayerState()) + ";" + std::to_string(playerCopy->getPlayerPos().x) +";ep;";
		const char* data = playPosMsg.c_str();
		m_udpCommSocket.send(data, playPosMsg.length(), m_tcpCommSocket.getRemoteAddress(), m_portNum);
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

#ifdef _WIN32
	strcpy_s(data, address.toString().c_str());
#elif __APPLE__
	strcpy(data, address.toString().c_str());
#endif // _WIN32

	
	
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
		//std::cout << "Recieved " << recieved << " bytes from " << sender << " on port " << port << std::endl;
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

bool ClientConnectionManager::AttemptConnection(std::string lobbyKey, std::string username, std::string password)
{
	ConnectionInfo serverToContact;
	if (m_detectedLobbies.find(lobbyKey) != m_detectedLobbies.end())
		serverToContact = m_detectedLobbies.find(lobbyKey)->second;
	else
		std::cerr << "ERROR: This lobby doesn't exist!" << std::endl;

	sf::Socket::Status status = m_tcpCommSocket.connect(serverToContact.address, serverToContact.port);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error in binding to tcp!" << std::endl;
	}

	std::string loginDetails = username + ";" + password + ";ep;";
	const char* loginPacket = loginDetails.c_str();
	m_tcpCommSocket.setBlocking(true);
	m_tcpCommSocket.send(loginPacket, loginDetails.length()); //send our login request

	char registerPacket[MAXBUFFSIZE];
	size_t recieved;
	if (m_tcpCommSocket.receive(registerPacket, MAXBUFFSIZE, recieved) != sf::Socket::Done)
	{

	}

	std::string initMsg(registerPacket, recieved);
	std::cout << "Recieved encrypted packet: " << initMsg << std::endl;
	//decrypt this secured accept packet
	for (int i = 0; i < initMsg.size(); ++i)
		initMsg[i] ^= 's';

	std::cout << "Decrypt resolved to: " << initMsg << std::endl;

	std::string delimiter(";");
	std::list<std::string> tokens = split(initMsg, ";");

	if (tokens.front() == "fail") //if the login failed
	{
		m_tcpCommSocket.disconnect();
		return false;
	}

	//first one is uniqueplayerid
	unsigned int playerID = std::stoul(tokens.front());
	tokens.pop_front();
	float xPos = std::stof(tokens.front());
	tokens.pop_front();
	int udpServerPort = std::stoi(tokens.front());
	tokens.pop_front();

	if (tokens.size() == 2) //password handle
	{

	}

	m_portNum = udpServerPort;

	status = m_udpCommSocket.bind(sf::Socket::AnyPort, serverToContact.address);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error in binding to udp!" << std::endl;
	}

	//recieved our acceptance packet, send the port we're going to listen on udp
	std::string portNum = "lp;" + std::to_string(m_udpCommSocket.getLocalPort()) + ";ep;";
	const char* port = portNum.c_str();

	m_udpCommSocket.send(port, portNum.length(), serverToContact.address, m_portNum);

	Application::instance()->WorldSystem()->Init(playerID, xPos, HEIGHT);
	Application::instance()->StateSystem()->SwitchScene(GAME_LOOP);
	StartClientCommunication();

	return true;
}

//log in with admin username and admin password
bool ClientConnectionManager::AttemptLocalConnection()
{

    std::cout << m_tcpCommSocket.getLocalPort() << std::endl;
    sf::Socket::Status status = m_tcpCommSocket.connect(sf::IpAddress::getLocalAddress(), 8080);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error in locally connecting to tcp!" << std::endl;
	}

	char registerPacket[MAXBUFFSIZE];
	size_t recieved;
	if (m_tcpCommSocket.receive(registerPacket, MAXBUFFSIZE, recieved) != sf::Socket::Done)
	{

	}

	std::string initMsg(registerPacket, recieved);

	std::cout << "Recieved encrypted packet: " << initMsg << std::endl;
	//decrypt this secured accept packet
	for (int i = 0; i < initMsg.size(); ++i)
		initMsg[i] ^= 's';

	std::cout << "Decrypt resolved to: " << initMsg << std::endl;

	std::string delimiter(";");
	std::list<std::string> tokens = split(initMsg, ";");

	//first one is uniqueplayerid
	unsigned int playerID = std::stoul(tokens.front());
	tokens.pop_front();
	float xPos = std::stof(tokens.front());
	tokens.pop_front();
	int udpServerPort = std::stoi(tokens.front());
	tokens.pop_front();

	if (tokens.size() == 2) //there's a password needed, do something here
	{

	}

	m_portNum = udpServerPort;

    status = m_udpCommSocket.bind(sf::Socket::AnyPort, sf::IpAddress::getLocalAddress());

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error in binding to udp!" << std::endl;
	}
    
    //recieved our acceptance packet, send the port we're going to listen on udp
    std::string portNum = "lp;" + std::to_string(m_udpCommSocket.getLocalPort()) + ";ep;";
    const char* port = portNum.c_str();
    
    m_udpCommSocket.send(port, portNum.length(), sf::IpAddress::getLocalAddress(), m_portNum);

	Application::instance()->WorldSystem()->Init(playerID, xPos, HEIGHT);
	Application::instance()->StateSystem()->SwitchScene(GAME_LOOP);
	StartClientCommunication();

	return true;
}

bool ClientConnectionManager::LaunchServerApplication(std::string lobbyName, std::string password)
{
#ifdef _WIN32
    std::string execPath = "\"" + Application::instance()->GetAppPath() + "MIG_SFML_SERVER.exe\"";
    //need to add quotations at either side here
    execPath.append(" \"");
    execPath.append(lobbyName);
    execPath.append("\" \"");
    execPath.append(password);
    execPath.append("\"");
    
    std::cout << execPath << std::endl;
    
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
    std::string execPath = Application::instance()->GetAppPath();
    std::string execComm = "/Library/Frameworks/Mono.framework/Commands/mono " + execPath + "migsfmlservermac.exe " + lobbyName + " " + password + " &";
    
    system(execComm.c_str());
    
//	pid_t processId;
//
//	if ((processId = fork()) == 0) 
//	{
//        std::string execComm = "/Library/Frameworks/Mono.framework/Commands/mono " + execPath + "migsfmlservermac.exe";
//		const char * argv[] = { execComm.c_str(), lobbyName.c_str(), password.c_str() };
//        
//		if (execve(execComm.c_str(), NULL, NULL) < 0)
//		{
//			perror("execv error");
//		}
//	}
//	else if (processId < 0) 
//	{
//		perror("fork error");
//		return false;
//	}
    
    sleep(2);
#endif

	return true;
}
