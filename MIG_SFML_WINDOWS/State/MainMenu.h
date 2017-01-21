#pragma once
#include "..\Base\BaseLoopClass.h"
#include "..\Base\stdafx.h"
#include "..\Base\Application.h"

enum ActiveMenu
{
	MAIN_ROOT,
	LOBBY_SEARCH,
	HOST_INFO
};

class MainMenu : public BaseLoopClass
{
public:
	MainMenu() {};
	MainMenu(tgui::Gui* gui, sf::RenderWindow* window);
	~MainMenu();

	void Init() override;
	void Simulate(float deltaTime) override;
	void Render() override;

	void SwitchMenu(ActiveMenu menu);

private:
	tgui::Gui* m_gui;
	ActiveMenu m_active = MAIN_ROOT;
};

