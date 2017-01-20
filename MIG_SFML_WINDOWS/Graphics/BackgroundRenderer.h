#pragma once

#ifdef _WIN32
#include "../Base/stdafx.h"
#include "../Base/BaseLoopClass.h"
#elif __APPLE__
#include "..\Base\stdafx.h"
#include "..\Base\BaseLoopClass.h"
#endif

#include "BackgroundEntity.h"

class BackgroundRenderer : public BaseLoopClass
{
public:
	BackgroundRenderer() {};
	BackgroundRenderer(sf::RenderWindow* window);
	~BackgroundRenderer();

	void Init() override {};
	void Init(float minX, float maxX, float minY, float maxY);
	void Simulate(float deltaTime) override;
	void Render() override;

private:
	BackgroundEntity m_clouds[10];
};

