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
	inline ActiveMenu GetActiveMenu() { return m_active; }
	inline sf::Font GetDefaultFont() { return m_defaultFont; }
	void InitHosting();
	void InitLobbySearch();
	void RefreshLobbySearch();
	void AddLobbyListing(std::string lobbyString, unsigned int lobbyNum);

	void AttemptJoinLobby();

#pragma region Thread Safe Queue Objects
	std::mutex lock;
	std::vector<std::string> lobbiesQueue;
#pragma endregion
private:
	sf::Font m_defaultFont;
	sf::Text lobbyCount;
	

	tgui::Gui* m_gui;
	ActiveMenu m_active = MAIN_ROOT;
};

