
#include "Application.h"

#include <iostream>

int main(int argc, char * argv[])
{
#ifdef _WIN32
	char basePath[255] = "";
	_fullpath(basePath, argv[0], sizeof(basePath));
	std::string execPath = basePath;
	execPath.erase(execPath.end() - 20, execPath.end());
	std::cout << execPath << std::endl;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#elif __APPLE__

#endif

	Application* app = Application::instance();
#ifdef _WIN32
	app->SetAppPath(execPath);
#endif
    
	app->Init();

	sf::Clock clock;
	float currentTime = clock.getElapsedTime().asSeconds();
	float dt = 1 / 60.0f, t = 0.0f;

	while (app->isRunning())
	{
		//Consume network queued messages and update the game simulation from them
		app->Update();

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		while (frameTime > 0.0f)
		{
			float deltaTime = std::min(frameTime, dt);

			//do anything that should be controlled by time instead of per frame here
			app->Simulate(deltaTime);
			frameTime -= deltaTime;
			t += deltaTime;
		}

		app->Render();
	}

	app->closing = true;

	return 0;
}
