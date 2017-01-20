#pragma once
#include "TextureLibrary.h"

class TexturesSingleton
{
public:
	static TexturesSingleton* instance()
	{
		static TexturesSingleton m_instance;

		return &m_instance;
	}

	TextureLibrary m_bgTextures;
	TextureLibrary m_playerTextures;

private:
	TexturesSingleton()
	{
		//eventually a folder loading function
		m_bgTextures.AddTexture("Art/cloud1.png", "cloud1");
		m_bgTextures.AddTexture("Art/cloud4.png", "cloud4");
		m_bgTextures.AddTexture("Art/cloud6.png", "cloud6");
		m_bgTextures.AddTexture("Art/menubgclearlayer.png", "menubg");

		m_playerTextures.AddSpritesheetTexture("Art/Players/p1_walk.png", sf::Rect<int>(0, 0, 73, 97), "p1walkcyclesheet");
		m_playerTextures.AddTexture("Art/Players/p1_duck.png", "p1duck");
		m_playerTextures.AddTexture("Art/Players/p1_duck.png", "p1jump");
	};
	~TexturesSingleton();

							  // C++ 11
							  // =======
							  // We can use the better technique of deleting the methods
							  // we don't want.
public:
	TexturesSingleton(TexturesSingleton const&) = delete;
	void operator=(TexturesSingleton const&) = delete;
};

