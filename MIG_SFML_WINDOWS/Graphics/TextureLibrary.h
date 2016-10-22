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
	sf::Texture* getTexture(int index) { if (m_library.size() - 1 >= index) return &m_library.at(index); else return nullptr; };

private:
	//static determination in an array would be nicer method of doing this, and use a std::pair with int for id reference
	std::vector<sf::Texture> m_library;
};

