#include "pch.h"
#include "EnemyComponent.h"
#include "Components/PhysicsComponent.h"
#include "Objects/Scene.h"

namespace nc
{
	void EnemyComponent::Create(void* data)
	{
		m_owner = static_cast<GameObject*>(data);
	}

	void EnemyComponent::Destroy()
	{

	}

	void EnemyComponent::Update()
	{
		GameObject* player = m_owner->m_scene->Find("Player");
		if (player)
		{
			Vector2 direction = player->m_transform.position - m_owner->m_transform.position;
			direction.Normalize();
			Vector2 force = direction * m_speed;

			PhysicsComponent* physicsComponent = m_owner->GetComponent<PhysicsComponent>();
			physicsComponent->ApplyForce(force);
		}
	}
	void EnemyComponent::Read(const rapidjson::Value& value)
	{
		json::Get(value, "speed", m_speed);
	}
}
