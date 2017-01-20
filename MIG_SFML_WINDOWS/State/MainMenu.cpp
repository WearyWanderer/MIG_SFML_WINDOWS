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
	tgui::Button::Ptr playButton = std::make_shared<tgui::Button>();
	playButton->setPosition(WIDTH / 2.2, HEIGHT / 2.2);
	playButton->setText("Play");
	m_gui->add(playButton, "Play Button");

	tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
	exitButton->setPosition(WIDTH / 2.2, HEIGHT / 1.8);
	exitButton->setText("Exit");

	m_gui->add(exitButton, "Exit Button");

	//playButton->connect() eventually tomorrow, switch to lobby searc - also use a singleton
	exitButton->connect("pressed", [&]() { m_window->close(); });
}

void MainMenu::Simulate(float deltaTime)
{

}

void MainMenu::Render()
{
	sf::Sprite background(*TexturesSingleton::instance()->m_bgTextures.getTexture("menubg"));
	m_window->draw(background);
}
