#pragma once
#include "stdafx.h"

class BaseLoopClass
{
public:
	BaseLoopClass() {};
	BaseLoopClass(sf::RenderWindow* window) : m_window(window) {};
	~BaseLoopClass();

	virtual void Init() = 0;
	virtual void Simulate(float deltaTime) = 0;
	virtual void Render() = 0;

protected:
	sf::RenderWindow* m_window;
};

