/*
Note:
Escape key uses held instead of pressed
*/
#include "pch.h"
#include "Graphics/Texture.h"
#include "Objects/GameObject.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/PlayerComponent.h"
#include "Core/Json.h"

nc::Engine engine;
nc::GameObject player;

namespace nc
{
using clock = std::chrono::high_resolution_clock;
using clock_duration = std::chrono::duration<clock::rep, std::nano>;
}

int main(int, char**)
{
	rapidjson::Document document;
	nc::json::Load("json.txt", document);

	std::string str;
	nc::json::Get(document, "string", str);
	std::cout << str << std::endl;

	bool b;
	nc::json::Get(document, "bool", b);
	std::cout << b << std::endl;

	int i1;
	nc::json::Get(document, "integer1", i1);
	std::cout << i1 << std::endl;

	int i2;
	nc::json::Get(document, "integer2", i2);
	std::cout << i2 << std::endl;

	float f;
	nc::json::Get(document, "float", f);
	std::cout << f << std::endl;

	nc::Vector2 v2;
	nc::json::Get(document, "vector2", v2);
	std::cout << v2 << std::endl;

	nc::Color color;
	nc::json::Get(document, "color", color);
	std::cout << color << std::endl;


	engine.Startup();
	player.Create(&engine);
	player.m_transform.position = { 400,300 };
	player.m_transform.angle = 45;

	nc::Component* component;
	component = new nc::PhysicsComponent;
	player.AddComponent(component);
	component->Create();

	component = new nc::SpriteComponent;
	player.AddComponent(component);
	component->Create();

	component = new nc::PlayerComponent;
	player.AddComponent(component);
	component->Create();

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	nc::Texture* background = engine.GetSystem<nc::ResourceManager>()->Get<nc::Texture>("background.png", engine.GetSystem<nc::Renderer>());

	nc::Vector2 velocity{ 0,0 };

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&event);
		engine.GetSystem<nc::InputSystem>()->Update();
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		//Update
		engine.Update();
		player.Update();
		engine.GetSystem<nc::Renderer>()->BeginFrame();
		if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eButtonState::HELD)
		{
			quit = true;
		}

		//draw
		engine.GetSystem<nc::Renderer>()->BeginFrame();
		background->Draw({ 0, 0 });
		player.Draw();

		engine.GetSystem<nc::Renderer>()->EndFrame();
	}
	engine.Shutdown();

	return 0;
}