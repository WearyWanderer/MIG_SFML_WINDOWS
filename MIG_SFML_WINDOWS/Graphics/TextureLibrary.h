#pragma once

#ifdef _WIN32
#include "..\Base\stdafx.h"
#elif __APPLE__
#include "../Base/stdafx.h"
#endif

#include <utility>
#include <map>

class TextureLibrary
{
public:
	TextureLibrary();
	~TextureLibrary();

	int AddTexture(std::string filePath, std::string textureName);
	int AddSpritesheetTexture(std::string filePath, sf::Rect<int> spriteSize, std::string textureName);

	sf::Texture* getTexture(std::string textureName);
	std::pair<sf::Texture, sf::Rect<int>>* getAnimatedTexture(std::string textureName);

private:
	//static determination in an array would be nicer method of doing this, and use a std::pair with int for id reference
	std::map <std::string, sf::Texture> m_library;
	std::map <std::string, std::pair<sf::Texture, sf::Rect<int>>> m_animatedLibrary;
};

