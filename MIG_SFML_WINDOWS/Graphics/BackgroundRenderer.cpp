#include "BackgroundRenderer.h"


BackgroundRenderer::BackgroundRenderer(TextureLibrary* bgTextures)
{
	for (int i = 0; i < 10; ++i)
	{
		m_clouds[i] = Entity(10, 10, bgTextures->getTexture(1), 2);
	}
}


BackgroundRenderer::~BackgroundRenderer()
{
}
