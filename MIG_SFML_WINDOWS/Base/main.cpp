#include "stdafx.h"
#include "Application.h"

#include <iostream>

int main()
{
	Application app;
	app.Init();

	sf::Clock clock;
	float currentTime = clock.getElapsedTime().asSeconds();
	float dt = 1 / 60.0f, t = 0.0f;

	while (app.isRunning())
	{
		//Consume network queued messages and update the game simulation from them
		app.Update();

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		while (frameTime > 0.0f)
		{
			float deltaTime = std::min(frameTime, dt);

			//do anything that should be controlled by time instead of per frame here
			app.Simulate(deltaTime);
			frameTime -= deltaTime;
			t += deltaTime;
		}

		app.Render();
	}

	return 0;
}