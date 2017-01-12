#pragma once

#ifdef _WIN32
#include "..\Base\stdafx.h"
#elif __APPLE__
#include "../Base/stdafx.h"
#endif

#include <utility>

class TextureLibrary
{
public:
	TextureLibrary();
	~TextureLibrary();

	int AddTexture(std::string filePath);
	int AddSpritesheetTexture(std::string filePath, sf::Rect<int> spriteSize);

	sf::Texture* getTexture(int index) { if (m_library.size() - 1 >= index) return &m_library.at(index); else return nullptr; };
	std::pair<sf::Texture, sf::Rect<int>>* getAnimatedTexture(int index) { if (m_animatedLibrary.size() - 1 >= index) return &m_animatedLibrary.at(index); else return nullptr; };

private:
	//static determination in an array would be nicer method of doing this, and use a std::pair with int for id reference
	std::vector<sf::Texture> m_library;
	std::vector <std::pair<sf::Texture, sf::Rect<int>>> m_animatedLibrary;
};

