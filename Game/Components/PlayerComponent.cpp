#include "pch.h"
#include "PlayerComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AudioComponent.h"

namespace nc
{
	void nc::PlayerComponent::Create(void* data)
	{
		m_owner = static_cast<GameObject*>(data);
		EventManager::Instance().Subscribe("CollsionEnter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), m_owner);
		EventManager::Instance().Subscribe("CollsionExit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), m_owner);
	}
	
	void nc::PlayerComponent::Destroy()
	{
	
	}
	
	void nc::PlayerComponent::Update()
	{
		auto contacts = m_owner->GetContactsWithTag("Floor");
		bool onGround = !contacts.empty();

		nc::Vector2 force{ 0,0 };
		//player controller
		if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_A) == nc::InputSystem::eButtonState::HELD)
		{
			force.x = -100;
		}
		if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_D) == nc::InputSystem::eButtonState::HELD)
		{
			force.x = 100;
		}
		if (onGround && m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_SPACE) == nc::InputSystem::eButtonState::PRESSED)
		{
			force.y = -1400;
			AudioComponent* audioComponent = m_owner->GetComponent<AudioComponent>();
			if (audioComponent)
			{
				AudioComponent* audioComponent = m_owner->GetComponent<AudioComponent>();
				audioComponent->SetSoundName("audio/jump.wav");
				audioComponent->Play();
			}
		}

		PhysicsComponent* component = m_owner->GetComponent<PhysicsComponent>();
		if (component)
		{
			component->ApplyForce(force);

			SpriteComponent* spriteComponent = m_owner->GetComponent<SpriteComponent>();
			Vector2 velocity = component->GetVelocity();
			if (velocity.x >= 0.1f) spriteComponent->Flip(false);
			if (velocity.x <= -0.1f) spriteComponent->Flip();
		}

	}

	void PlayerComponent::OnCollisionEnter(const Event& event)
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(event.sender);

		std::cout << "collision enter: " << gameObject->m_name << std::endl;

		if (gameObject->m_tag == "Enemy")
		{
			AudioComponent* audioComponent = m_owner->GetComponent<AudioComponent>();
			audioComponent->SetSoundName("audio/Grunt.wav");
			audioComponent->Play();

			Event event;
			event.type = "PlayerDead";
			int score = 300;
			event.data = &score;
			EventManager::Instance().Notify(event);

			m_owner->m_flags[GameObject::eFlags::DESTROY] = true;
		}

		if (gameObject->m_tag == "Coin")
		{
			AudioComponent* audioComponent = m_owner->GetComponent<AudioComponent>();
			audioComponent->SetSoundName("audio/Coin.wav");
			audioComponent->Play();

			gameObject->m_flags[GameObject::eFlags::DESTROY] = true;
		}
	}
	void PlayerComponent::OnCollisionExit(const Event& event)
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(event.sender);

		std::cout << "collision exit: " << gameObject->m_name << std::endl;
	}
}
