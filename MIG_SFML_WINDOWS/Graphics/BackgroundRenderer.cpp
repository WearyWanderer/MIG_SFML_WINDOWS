#include "BackgroundRenderer.h"
#include "GraphicsUtilities.h"
#include <iostream>

BackgroundRenderer::BackgroundRenderer(sf::RenderWindow* window) : BaseLoopClass(window)
{
	m_window = window;
}


BackgroundRenderer::~BackgroundRenderer()
{
}

void BackgroundRenderer::Init(float minX, float maxX, float minY, float maxY)
{
	TextureLibrary* bgTextures = &TexturesSingleton::instance()->m_bgTextures;
#pragma region Clouds
	unsigned char cloudOptions = 0;
	cloudOptions |= wrapAroundScreen;


	for (int i = 0; i < 10; ++i)
	{
		m_clouds[i] = BackgroundEntity(rand() % WIDTH, rand() % (HEIGHT - 100), bgTextures->getTexture("cloud1"), 2, static_cast <float> (rand() % 20) + 50.0f, cloudOptions);
		gfx::ResetOriginToCenter(&m_clouds[i].m_sprite);
		m_clouds[i].m_sprite.setScale(0.5f, 0.5f);
	}
#pragma endregion
}

void BackgroundRenderer::Simulate(float deltaTime)
{
	sf::Rect<float> screenRect;
	screenRect.left = (m_window->getView().getCenter().x - (m_window->getView().getSize().x / 2)) - 10.0f; //extra padding so that loops work
	screenRect.top = (m_window->getView().getCenter().y - (m_window->getView().getSize().y / 2)) - 10.0f;
	screenRect.width = m_window->getView().getSize().x + 10.0f;
	screenRect.height = m_window->getView().getSize().y + 10.0f;

	for (int i = 0; i < 10; ++i)
	{
		m_clouds[i].m_sprite.move(m_clouds[i].m_speed * deltaTime, 0.0f);

		if (!m_clouds[i].m_sprite.getGlobalBounds().intersects(screenRect))
		{
			m_clouds[i].m_sprite.setPosition(screenRect.left, m_clouds[i].m_sprite.getPosition().y);
			m_clouds[i].m_sprite.move(-30.0f, 0.0f);
		}
	}
}

void BackgroundRenderer::Render()
{
	for (Entity i : m_clouds)
		m_window->draw(i.m_sprite);
}
