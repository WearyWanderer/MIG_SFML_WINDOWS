#pragma once
#include "..\Base\BaseLoopClass.h"
#include "..\Base\stdafx.h"


class MainMenu : public BaseLoopClass
{
public:
	MainMenu() {};
	MainMenu(tgui::Gui* gui, sf::RenderWindow* window);
	~MainMenu();

	void Init() override;
	void Simulate(float deltaTime) override;
	void Render() override;

private:
	tgui::Gui* m_gui;
};

