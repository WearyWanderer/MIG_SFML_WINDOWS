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

	m_bgTextures.AddTexture("Art/cloud1.png");
	m_bgTextures.AddTexture("Art/cloud4.png");
	m_bgTextures.AddTexture("Art/cloud6.png");

	m_playerTextures.AddSpritesheetTexture("Art/Players/p1_walk.png", sf::Rect<int>(0,0,72,96));

	m_bgRenderer = BackgroundRenderer(&m_bgTextures, &m_window);
	m_mainWorld = World(&m_playerTextures, &m_window);
}

void Application::Update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();
	}
}

void Application::Simulate(float deltaTime)
{
	m_bgRenderer.Simulate(deltaTime);
	m_mainWorld.Simulate(deltaTime);
}

void Application::Render()
{
	m_window.clear(sf::Color(142, 185, 255, 255));
	m_bgRenderer.Render();
	m_mainWorld.Render();
	m_window.display();
}
