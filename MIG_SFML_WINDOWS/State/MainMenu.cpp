#include "MainMenu.h"

MainMenu::MainMenu(tgui::Gui* gui, sf::RenderWindow* window) : BaseLoopClass(window)
{
	m_gui = gui;
	m_defaultFont.loadFromFile("Art/KenPixelHigh.ttf");
	lobbyCount.setFont(m_defaultFont);
	lobbyCount.setFillColor(sf::Color::Black);
	
}


MainMenu::~MainMenu()
{

}

void MainMenu::Init() 
{
	SwitchMenu(MAIN_ROOT);
	lobbiesQueue.reserve(100); //probably unlikely to have more than 100 lobbies, tweakable anyway
}

void MainMenu::Simulate(float deltaTime)
{
	if (!lobbiesQueue.empty())
	{
		std::shared_ptr<tgui::ListBox> list = m_gui->get<tgui::ListBox>("Lobby Listings");
		for each (auto i in lobbiesQueue)
			list->addItem(i);
		
		lobbiesQueue.clear();
	}
}

void MainMenu::Render()
{
	sf::Sprite background(*TexturesSingleton::instance()->m_bgTextures.getTexture("menubg"));
	m_window->draw(background);
	m_window->draw(lobbyCount);
}

void MainMenu::SwitchMenu(ActiveMenu menu)
{
	m_active = menu;
	m_gui->removeAllWidgets();
	lobbyCount.setString("");

	switch (m_active)
	{
	case MAIN_ROOT:
		{
			tgui::Button::Ptr hostButton = std::make_shared<tgui::Button>();
			hostButton->setPosition(WIDTH / 2.2, HEIGHT / 2.2);
			hostButton->setText("Host");

			tgui::Button::Ptr playButton = std::make_shared<tgui::Button>();
			playButton->setPosition(WIDTH / 2.2, HEIGHT / 1.9);
			playButton->setText("Play");

			tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
			exitButton->setPosition(WIDTH / 2.2, HEIGHT / 1.6);
			exitButton->setText("Exit");

			m_gui->add(playButton, "Play Button");
			m_gui->add(exitButton, "Exit Button");
			m_gui->add(hostButton, "Host Button");

			//playButton->connect() eventually tomorrow, switch to lobby searc - also use a singleton
			hostButton->connect("pressed", &MainMenu::SwitchMenu, this, HOST_INFO);
			//playButton->connect("pressed", &StateManager::SwitchScene, Application::instance()->StateSystem(), GAME_LOOP);
			playButton->connect("pressed", &MainMenu::InitLobbySearch, this);
			exitButton->connect("pressed", [&]() { m_window->close(); });
		}
		break;
	case LOBBY_SEARCH:
		{
			lobbyCount.setString("Online Lobbies: 0");
			lobbyCount.setPosition(WIDTH / 2 - (lobbyCount.getLocalBounds().width / 2), HEIGHT / 4.85);

			auto lobbyList = std::make_shared<tgui::ListBox>();
			lobbyList->setSize(WIDTH / 3, 120);
			lobbyList->setItemHeight(24);
			lobbyList->setPosition((WIDTH / 2) - ((WIDTH / 3) / 2), HEIGHT / 4);
			lobbyList->addItem("Lobby Name			|		  Players		|			Ping(ms)", "Legend");

			tgui::Button::Ptr refreshButton = std::make_shared<tgui::Button>();
			refreshButton->setPosition((WIDTH / 2) - (refreshButton->getSize().x) - 5.0f, HEIGHT / 2.1);
			refreshButton->setText("Refresh");

			tgui::Button::Ptr joinButton = std::make_shared<tgui::Button>();
			joinButton->setPosition((WIDTH / 2) + 5.0f, HEIGHT / 2.1);
			joinButton->setText("Join");

			tgui::Button::Ptr backButton = std::make_shared<tgui::Button>();
			backButton->setPosition(((WIDTH / 2) - (backButton->getSize().x / 2)), HEIGHT / 1.85);
			backButton->setText("Back");

			m_gui->add(refreshButton);
			m_gui->add(joinButton);
			m_gui->add(backButton);
			m_gui->add(lobbyList, "Lobby Listings");

			joinButton->connect("pressed", &MainMenu::AttemptJoinLobby, this);
			refreshButton->connect("pressed", &MainMenu::RefreshLobbySearch, this);
			backButton->connect("pressed", &MainMenu::SwitchMenu, this, MAIN_ROOT);
		}
		break;
	case HOST_INFO:
		{
			tgui::EditBox::Ptr editBoxUsername = std::make_shared<tgui::EditBox>();
			editBoxUsername->setSize(WIDTH * 2 / 4, HEIGHT / 10);
			editBoxUsername->setPosition((WIDTH / 2) - ((WIDTH * 2 / 4) / 2), HEIGHT / 4);
			editBoxUsername->setDefaultText("Lobby Name");

			tgui::EditBox::Ptr editBoxPassword = std::make_shared<tgui::EditBox>();
			editBoxPassword->setSize(WIDTH * 2 / 4, HEIGHT / 10);
			editBoxPassword->setPosition((WIDTH / 2) - ((WIDTH * 2 / 4) / 2), HEIGHT * 5 / 13);
			editBoxPassword->setPasswordCharacter('*');
			editBoxPassword->setDefaultText("Password (Optional)");

			tgui::Button::Ptr hostButton = std::make_shared<tgui::Button>();
			hostButton->setPosition(((WIDTH / 2) - hostButton->getSize().x) - 5, HEIGHT / 1.8);
			hostButton->setText("Host");

			tgui::Button::Ptr backButton = std::make_shared<tgui::Button>();
			backButton->setPosition((WIDTH / 2) + 5, HEIGHT / 1.8);
			backButton->setText("Back");

			m_gui->add(editBoxUsername, "Lobby Name");
			m_gui->add(editBoxPassword, "Password");
			m_gui->add(backButton, "Back Button");
			m_gui->add(hostButton, "Host Button");

			hostButton->connect("pressed", &StateManager::SwitchScene, Application::instance()->StateSystem(), LOADING_SCREEN);
			backButton->connect("pressed", &MainMenu::SwitchMenu, this, MAIN_ROOT);
		}
		break;
	default:
		break;
	}
}

void MainMenu::InitLobbySearch()
{
	SwitchMenu(LOBBY_SEARCH);
	Application::instance()->Client()->BroadcastLobbySearch();
}

void MainMenu::RefreshLobbySearch()
{
	lobbyCount.setString("Online Lobbies: 0");
	lobbyCount.setPosition(WIDTH / 2 - (lobbyCount.getLocalBounds().width / 2), HEIGHT / 4.85);

	std::shared_ptr<tgui::ListBox> list = m_gui->get<tgui::ListBox>("Lobby Listings");
	list->removeAllItems();
	list->addItem("Lobby Name			|		  Players		|			Ping(ms)", "Legend");
	
	Application::instance()->Client()->BroadcastLobbySearch();
}

void MainMenu::AddLobbyListing(std::string lobbyString, unsigned int lobbyNum)
{
	lobbyCount.setString("Online Lobbies: " + std::to_string(lobbyNum));
	lobbiesQueue.push_back(lobbyString);
}

void MainMenu::AttemptJoinLobby()
{
	std::shared_ptr<tgui::ListBox> list = m_gui->get<tgui::ListBox>("Lobby Listings");

	if (!(list->getSelectedItemId() == "Title") && list->getSelectedItem() != "")
	{
		Application::instance()->Client()->AttemptConnection(list->getSelectedItem());
	}
}
