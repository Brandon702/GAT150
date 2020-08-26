#include "pch.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/RenderComponent.h"
#include "ObjectFactory.h"

namespace nc
{
	GameObject::GameObject(const GameObject& other)
	{
		m_name = other.m_name;
		m_tag = other.m_tag;
		m_flags = other.m_flags;
		m_lifetime = other.m_lifetime;

		m_transform = other.m_transform;
		m_engine = other.m_engine;

		for (auto component : other.m_componenets)
		{
			Component* clone = static_cast<Component*>(component->Clone());
			clone->m_owner = this;
			AddComponent(clone);
		}
	}
	void GameObject::Create(void* data)
	{
		m_engine = static_cast<Engine*>(data);
	}

	void GameObject::Destroy()
	{
		RemoveAllComponents();
	}

	void GameObject::ReadComponents(const rapidjson::Value& value)
	{
		for (rapidjson::SizeType i = 0; i < value.Size(); i++)
		{
			const rapidjson::Value& componentValue = value[i];
			if (componentValue.IsObject())
			{
				std::string typeName;
				json::Get(componentValue, "type", typeName);

				Component* component = ObjectFactory::Instance().Create<Component>(typeName);
				if (component)
					{
						component->Create(this);
						component->Read(componentValue);
						AddComponent(component);
					}
			}
		}

	}

	void GameObject::Read(const rapidjson::Value& value)
	{
		json::Get(value, "name", m_name);
		json::Get(value, "tag", m_tag);

		bool transient = m_flags[eFlags::TRANSIENT];
		json::Get(value, "transient", transient);
		m_flags[eFlags::TRANSIENT] = transient;

		json::Get(value, "lifetime", m_lifetime);

		json::Get(value, "position", m_transform.position);
		json::Get(value, "scale", m_transform.scale);
		json::Get(value, "angle", m_transform.angle);

		if (value.HasMember("Components"))
		{
		const rapidjson::Value& componentsValue = value["Components"];
			if (componentsValue.IsArray())
			{
				ReadComponents(componentsValue);
			}
		}
	}

	void GameObject::Update()
	{
		for (auto component : m_componenets)
		{
			component->Update();
		}

		if (m_flags[eFlags::TRANSIENT])
		{
			m_lifetime -= m_engine->GetTimer().DeltaTime();
			m_flags[eFlags::DESTROY] = (m_lifetime <= 0);
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