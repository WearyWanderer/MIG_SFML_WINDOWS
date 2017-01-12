#include "TextureLibrary.h"
#include <iostream>


TextureLibrary::TextureLibrary()
{
}


TextureLibrary::~TextureLibrary()
{
}

sf::Texture* TextureLibrary::getTexture(std::string textureName)
{
	if(m_library.find(textureName) != m_library.end())
		return &m_library.find(textureName)->second;
	else return nullptr;
}

std::pair<sf::Texture, sf::Rect<int>>* TextureLibrary::getAnimatedTexture(std::string textureName)
{
	if (m_animatedLibrary.find(textureName) != m_animatedLibrary.end())
		return &m_animatedLibrary.find(textureName)->second;
	else return nullptr;
}

int TextureLibrary::AddTexture(std::string filePath, std::string textureName)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filePath))
		std::cerr << "ERROR LOADING TEXTURE -" << filePath << std::endl;

	texture.setSmooth(true);
	m_library.insert(std::pair<std::string, sf::Texture>(textureName, texture));
	return m_library.size() - 1;
}

int TextureLibrary::AddSpritesheetTexture(std::string filePath, sf::Rect<int> spriteSize, std::string textureName)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filePath))
		std::cerr << "ERROR LOADING TEXTURE -" << filePath << std::endl;

	texture.setSmooth(true);
	m_animatedLibrary.insert(std::pair<std::string, std::pair<sf::Texture, sf::Rect<int>>>(textureName, std::pair<sf::Texture, sf::Rect<int>>(texture, spriteSize)));
	return m_library.size() - 1;
}
