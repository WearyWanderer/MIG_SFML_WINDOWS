#pragma once

#ifdef _WIN32
#include "../Base/stdafx.h"
#elif __APPLE__
#include "..\Base\stdafx.h"
#endif

#include "TextureLibrary.h"
#include "BackgroundEntity.h"

class BackgroundRenderer
{
public:
	BackgroundRenderer() {};
	BackgroundRenderer(TextureLibrary* bgTextures, sf::RenderWindow* window);
	~BackgroundRenderer();

	void Init(float minX, float maxX, float minY, float maxY);
	void Simulate(float deltaTime);
	void Render();

private:
	BackgroundEntity m_clouds[10];
	sf::RenderWindow* m_window;
};

