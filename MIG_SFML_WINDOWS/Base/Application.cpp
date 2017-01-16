#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	srand(time(NULL));
	m_window.create(sf::VideoMode(700, 700), "SFML works!");

	m_bgTextures.AddTexture("Art/cloud1.png", "cloud1");
	m_bgTextures.AddTexture("Art/cloud4.png", "cloud4");
	m_bgTextures.AddTexture("Art/cloud6.png", "cloud6");

	m_playerTextures.AddSpritesheetTexture("Art/Players/p1_walk.png", sf::Rect<int>(0,0,73,97), "p1walkcyclesheet");
	m_playerTextures.AddTexture("Art/Players/p1_duck.png", "p1duck");
	m_playerTextures.AddTexture("Art/Players/p1_duck.png", "p1jump");


	m_bgRenderer = BackgroundRenderer(&m_bgTextures, &m_window);
	m_mainWorld = World(&m_playerTextures, &m_window);
}

//Apply network related stuff here, consume any packets or messages that we need to
void Application::Update()
{
	m_input.KeyboardInput(&m_state, &m_mainWorld);

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();
	}
}

//local simulation, gravity, damage, timer countdown etc. here
void Application::Simulate(float deltaTime)
{
	m_bgRenderer.Simulate(deltaTime);
	m_mainWorld.Simulate(deltaTime);
}

void Application::Render()
{
	m_window.clear(sf::Color(0, 185, 255, 255));
	//m_window.clear(sf::Color(142, 185, 255, 255)); sky cols
	m_bgRenderer.Render();
	m_mainWorld.Render();
	m_window.display();
}
