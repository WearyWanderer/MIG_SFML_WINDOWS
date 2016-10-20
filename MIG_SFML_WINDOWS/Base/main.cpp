#include "stdafx.h"

#include <iostream>

#include "../Graphics/TextureLibrary.h"
#include "../Entity.h"

int main()
{

#pragma region Startup (window, clock etc.)
	sf::RenderWindow window(sf::VideoMode(700, 700), "SFML works!");
	sf::Clock clock;
	float currentTime = clock.getElapsedTime().asSeconds();
	float dt = 1 / 60.0f, t = 0.0f;
#pragma endregion

	TextureLibrary bgTextures;
	bgTextures.AddTexture("Art/cloud1.png");
	bgTextures.AddTexture("Art/cloud4.png");
	bgTextures.AddTexture("Art/cloud6.png");

	sf::Texture* texture = bgTextures.getTexture(1);

	Entity testCloud(10.0f, 10.0f, texture);
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		while (frameTime > 0.0f)
		{
			float deltaTime = std::min(frameTime, dt);
			//do anything that should be controlled by time instead of per frame here
			frameTime -= deltaTime;
			t += deltaTime;
		}


#pragma region Rendering
		window.clear(sf::Color(142, 185, 255, 255));
		window.draw(testCloud.m_sprite);
		window.display();
#pragma endregion
	}

	return 0;
}