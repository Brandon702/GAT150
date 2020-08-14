#include "pch.h"
#include "Engine.h"

namespace nc
{
	bool nc::Engine::Startup()
	{
		m_systems.push_back(new Renderer);
		m_systems.push_back(new InputSystem);
		m_systems.push_back(new ResourceManager);
		for (auto system : m_systems)
		{
			system->Startup();
		}
		//Create Window
		GetSystem<Renderer>()->Create("GAT150", 800, 600);

		return true;
	}
	
	void nc::Engine::Shutdown()
	{
		for (auto system : m_systems)
		{
			system->Shutdown();
			delete system;
		}
		IMG_Quit();
		SDL_Quit();
	}
	
	void nc::Engine::Update()
	{
		m_timer.Tick();
		for (auto system : m_systems)
		{
			system->Update();
		}
	}
}