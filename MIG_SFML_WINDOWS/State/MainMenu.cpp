#include "MainMenu.h"

MainMenu::MainMenu(tgui::Gui* gui, sf::RenderWindow* window) : BaseLoopClass(window)
{
	m_gui = gui;
}


MainMenu::~MainMenu()
{
}

void MainMenu::Init() 
{
	SwitchMenu(MAIN_ROOT);
}

void MainMenu::Simulate(float deltaTime)
{

}

void MainMenu::Render()
{
	sf::Sprite background(*TexturesSingleton::instance()->m_bgTextures.getTexture("menubg"));
	m_window->draw(background);
}

void MainMenu::SwitchMenu(ActiveMenu menu)
{
	m_active = menu;
	m_gui->removeAllWidgets();

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
			playButton->connect("pressed", &StateManager::SwitchScene, Application::instance()->StateSystem(), GAME_LOOP);
			exitButton->connect("pressed", [&]() { m_window->close(); });
		}
		break;
	case LOBBY_SEARCH:
		{
			auto lobbyList = std::make_shared<tgui::ListBox>();
			lobbyList->setSize(250, 120);
			lobbyList->setItemHeight(24);
			lobbyList->setPosition((WIDTH / 2) - 125, (HEIGHT / 2) + 170);
			lobbyList->addItem("Item 1");
			lobbyList->addItem("Item 2");
			lobbyList->addItem("Item 3");

			m_gui->add(lobbyList);
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
