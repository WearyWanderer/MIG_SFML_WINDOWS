#include "Application.h"

Application::Application()
{
	srand(time(NULL));
	m_window.create(sf::VideoMode(WIDTH, HEIGHT), "Multiplayer ICA2 Prototype");
	m_gui.setWindow(m_window);
	m_menu = std::make_shared<MainMenu>(&m_gui, &m_window);
	m_state = std::make_shared<StateManager>();
	m_bgRenderer = std::make_shared<BackgroundRenderer>(&m_window);
	m_mainWorld = std::make_shared<World>(&m_window);
}

Application::~Application()
{
	//m_mainWorld.~World();
}

void Application::Init()
{
	m_menu->Init();
	m_bgRenderer->Init(0, 0, 0, 0); //eventually this init will need to be elsewhere and will take the rect of the players spawned coordinates
}

//Apply network related stuff here, consume any packets or messages that we need to
void Application::Update()
{
	m_input->KeyboardInput(StateSystem(), WorldSystem());

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		m_gui.handleEvent(event);
	}

	if (m_state->getCurrentScene() == GAME_LOOP /*&& something about our network connection being correctly set up, then next loop check if we have any outstanding messages to apply*/)
	{

	}
}

//local simulation, gravity, damage, timer countdown etc. here
void Application::Simulate(float deltaTime)
{
	if (m_state->getCurrentScene() == GAME_LOOP)
	{
		m_bgRenderer->Simulate(deltaTime);
		m_mainWorld->Simulate(deltaTime);
	}
}

void Application::Render()
{
	m_window.clear(sf::Color(0, 185, 255, 255));
	//m_window.clear(sf::Color(142, 185, 255, 255)); sky cols

	switch (m_state->getCurrentScene())
	{
	case LOADING_SCREEN:

		break;
	case GAME_LOOP:
		m_bgRenderer->Render();
		m_mainWorld->Render();
		break;
	case MAIN_MENU:
	default:
		m_menu->Render();
		m_gui.draw();
		break;
	}

	m_window.display();
}
