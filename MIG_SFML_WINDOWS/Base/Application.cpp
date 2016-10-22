#include "Application.h"


Application::Application()
{
}


Application::~Application()
{
}

void Application::Init()
{
	m_window.create(sf::VideoMode(700, 700), "SFML works!");

	m_bgTextures.AddTexture("Art/cloud1.png");
	m_bgTextures.AddTexture("Art/cloud4.png");
	m_bgTextures.AddTexture("Art/cloud6.png");


}

void Application::Update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();
	}

	//TEMP
	sf::Texture* texture = m_bgTextures.getTexture(1);
	Entity testCloud(10.0f, 10.0f, texture, 2);
}

void Application::Simulate(float deltaTime)
{

}

void Application::Render()
{
	m_window.clear(sf::Color(142, 185, 255, 255));
	//window.draw(testCloud.m_sprite);
	m_window.display();
}
