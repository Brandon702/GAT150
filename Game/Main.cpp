#include "pch.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include <SDL_image.h>
#include "InputSystem.h"
#include "Resources/ResourceManager.h"
#include "Core/time.h"
#include "Math/Math.h"

nc::Renderer renderer;
nc::ResourceManager resourceManager;
nc::InputSystem inputSystem;
nc::FrameTimer timer;

namespace nc
{
using clock = std::chrono::high_resolution_clock;
using clock_duration = std::chrono::duration<clock::rep, std::nano>;
}

int main(int, char**)
{
	/*nc::Timer timer;
	for (size_t i = 0; i < 1000; i++) { std::sqrt(rand() & 100); }

	std::cout << timer.ElapsedTicks() << std::endl;
	std::cout << timer.ElapsedSeconds() << std::endl;*/

	renderer.Startup();
	resourceManager.Startup();
	inputSystem.Startup();
	renderer.Create("GAT150 - Game", 800, 600);

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	nc::Texture* background = resourceManager.Get<nc::Texture>("background.png", &renderer);
	nc::Texture* car = resourceManager.Get<nc::Texture>("cars.png", &renderer);

	nc::Vector2 position{ 400,300 };
	float angle{ 0 };
	nc::Vector2 velocity{ 0,0 };

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&event);
		inputSystem.Update();
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		//Update
		timer.Tick();
		resourceManager.Update();
		renderer.BeginFrame();
		quit = (inputSystem.GetButtonState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eButtonState::PRESSED);

		//player controller
		if (inputSystem.GetButtonState(SDL_SCANCODE_A) == nc::InputSystem::eButtonState::HELD)
		{
			angle = angle - 200.0f * timer.DeltaTime();
		}
		if (inputSystem.GetButtonState(SDL_SCANCODE_D) == nc::InputSystem::eButtonState::HELD)
		{
			angle = angle + 200.0f * timer.DeltaTime();
		}
		//physics
		nc::Vector2 force{ 0,0 };
		if (inputSystem.GetButtonState(SDL_SCANCODE_W) == nc::InputSystem::eButtonState::HELD)
		{
			force = nc::Vector2::forward * 1000.0f;
		}
		force = nc::Vector2::Rotate(force, nc::DegreesToRadians(angle));
		velocity = velocity + force * timer.DeltaTime();
		velocity = velocity * 0.93f;
		position = position + velocity * timer.DeltaTime();



		//draw
		renderer.BeginFrame();
		background->Draw({ 0, 0 });

		//Player sprite draw
		car->Draw({64,110,60,112}, position, { 1,1 }, angle);

		renderer.EndFrame();
	}
	IMG_Quit();
	resourceManager.Shutdown();
	inputSystem.Shutdown();
	SDL_Quit();

	return 0;
}