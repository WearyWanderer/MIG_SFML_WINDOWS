#pragma once

#ifdef _WIN32
#include "../Base/stdafx.h"
#include "../Entity.h"
#elif __APPLE__
#include "..\Base\stdafx.h"
#include "..\Entity.h"
#endif

#include "TextureLibrary.h"

class BackgroundRenderer
{
public:
	BackgroundRenderer(TextureLibrary* bgTextures);
	~BackgroundRenderer();

private:
	Entity m_clouds[10];
};

