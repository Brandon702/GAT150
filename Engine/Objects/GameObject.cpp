#include "pch.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/RenderComponent.h"

namespace nc
{
	void GameObject::Create(void* data)
	{
		m_engine = static_cast<Engine*>(data);
	}

	void GameObject::Destroy()
	{
		RemoveAllComponents();
	}

	void GameObject::Read(const rapidjson::Value& value)
	{
		json::Get(value, "position", m_transform.position);
		json::Get(value, "scale", m_transform.scale);
		json::Get(value, "angle", m_transform.angle);
	}

	void GameObject::Update()
	{
		for (auto component : m_componenets)
		{
			component->Update();
		}
	}

	void GameObject::Draw()
	{
		RenderComponent* component = GetComponent<RenderComponent>();
		if (component)
		{
			component->Draw();
		}
	}
	void GameObject::AddComponent(Component* component)
	{
		component->m_owner = this;
		m_componenets.push_back(component);
	}
	void GameObject::RemoveComponent(Component* component)
	{
		auto iter = std::find(m_componenets.begin(), m_componenets.end(), component);
		if (iter != m_componenets.end())
		{
			(*iter)->Destroy();
			delete (*iter);
		}
	}
	void GameObject::RemoveAllComponents()
	{
		for (auto component : m_componenets)
		{
			component->Destroy();
			delete component;
		}
	}
}