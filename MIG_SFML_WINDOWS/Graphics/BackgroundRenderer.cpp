#include "BackgroundRenderer.h"
#include "GraphicsUtilities.h"
#include <iostream>

BackgroundRenderer::BackgroundRenderer(TextureLibrary* bgTextures, sf::RenderWindow* window)
{
	m_window = window;

#pragma region Clouds
	unsigned char cloudOptions = 0;
	cloudOptions |= wrapAroundScreen;


	for (int i = 0; i < 10; ++i)
	{
		m_clouds[i] = BackgroundEntity(50 * i, 50 * i, bgTextures->getTexture(1), 2, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5)) + 5.0f, cloudOptions);
		gfx::ResetOriginToCenter(&m_clouds[i].m_sprite);
		m_clouds[i].m_sprite.setScale(0.5f, 0.5f);
	}
#pragma endregion
}


BackgroundRenderer::~BackgroundRenderer()
{
}

void BackgroundRenderer::Init(float minX, float maxX, float minY, float maxY)
{

}

void BackgroundRenderer::Simulate(float deltaTime)
{
	for (int i = 0; i < 10; ++i)
	{
		m_clouds[i].m_sprite.move(m_clouds[i].m_speed * deltaTime, 0.0f);
	}
}

void BackgroundRenderer::Render()
{
	for (Entity i : m_clouds)
		m_window->draw(i.m_sprite);
}
