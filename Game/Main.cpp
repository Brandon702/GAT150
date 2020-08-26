/*
Note:
Factory.h
*/
#include "pch.h"
#include "Graphics/Texture.h"
#include "Objects/GameObject.h"
#include "Components/PlayerComponent.h"
#include "Core/Json.h"
#include "Core/Factory.h"
#include "Objects/ObjectFactory.h"
#include "Objects/Scene.h"

nc::Engine engine;
nc::Scene scene;

namespace nc
{
using clock = std::chrono::high_resolution_clock;
using clock_duration = std::chrono::duration<clock::rep, std::nano>;
}

int main(int, char**)
{
	engine.Startup();

	nc::ObjectFactory::Instance().Initialize();
	nc::ObjectFactory::Instance().Register("PlayerComponent", new nc::Creator<nc::PlayerComponent, nc::Object>);

	scene.Create(&engine);
	rapidjson::Document document;
	nc::json::Load("scene.txt", document);
	scene.Read(document);

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		//Update
		engine.Update();
		scene.Update();
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eButtonState::PRESSED)
		{
			quit = true;
		}

		//draw
		engine.GetSystem<nc::Renderer>()->BeginFrame();
		scene.Draw();

		engine.GetSystem<nc::Renderer>()->EndFrame();
	}
	scene.Destroy();
	engine.Shutdown();

	return 0;
}