#include "TextureLibrary.h"
#include <iostream>


TextureLibrary::TextureLibrary()
{
}


TextureLibrary::~TextureLibrary()
{
}

int TextureLibrary::AddTexture(std::string filePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filePath))
	{
		std::cerr << "ERROR LOADING TEXTURE -" << filePath << std::endl;
	}

	texture.setSmooth(true);
	m_library.push_back(texture);
	return m_library.size() - 1;
}

int TextureLibrary::AddSpritesheetTexture(std::string filePath, sf::Rect<int> spriteSize)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filePath))
	{
		std::cerr << "ERROR LOADING TEXTURE -" << filePath << std::endl;
	}

	texture.setSmooth(true);
	m_animatedLibrary.push_back(std::pair<sf::Texture,sf::Rect<int>>(texture,spriteSize));
	return m_library.size() - 1;
}
