#include "stdafx.h"

#include "../Graphics/TextureLibrary.h"
#include "../Entity.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(700, 700), "SFML works!");
	
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
		testCloud.m_sprite.move(1, 1);
		window.clear(sf::Color(142, 185, 255, 255));
		window.draw(testCloud.m_sprite);
		window.display();
	}

	return 0;
}