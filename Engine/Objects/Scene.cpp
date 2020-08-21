#include "pch.h"
#include "Scene.h"
#include "ObjectFactory.h"
#include "GameObject.h"

namespace nc
{
	void Scene::Create(void* data)
	{
		m_engine = static_cast<Engine*>(data);
	}
	void Scene::Destroy()
	{
		RemoveAllGameObjects();
	}
	void Scene::Read(const rapidjson::Value& value)
	{
	/*
		const rapidjson::Value& objectValue = value["GameObject"];
		if (objectValue.IsObject())
		{
			std::string typeName;
			json::Get(objectValue, "type", typeName);
			nc::GameObject* gameObject = ObjectFactory::Instance().Create<GameObject>(typeName);
			
			if (gameObject)
				{
					gameObject->Create(m_engine);
					gameObject->Read(objectValue);

					AddGameObject(gameObject);
				}
		}*/
		const rapidjson::Value& objectsValue = value["GameObjects"];
		if (objectsValue.IsArray())
		{
			ReadGameObjects(objectsValue);
		}

	}

	void Scene::ReadGameObjects(const rapidjson::Value& value)
	{
		for (rapidjson::SizeType i = 0; i < value.Size(); i++)
		{
			const rapidjson::Value& objectValue = value[i];
			if (objectValue.IsObject())
			{
				std::string typeName;
				// read game object “type” name from json (Get)
				json::Get(objectValue, "type", typeName);
				nc::GameObject* gameObject = ObjectFactory::Instance().Create<GameObject>(typeName);

				if (gameObject)
				{
					gameObject->Create(m_engine);
					gameObject->Read(objectValue);

					AddGameObject(gameObject);
				}
			}
		}
	}


	void Scene::Update()
	{
		for (auto gameObject : m_gameObjects)
		{
			gameObject->Update();
		}
	}
	void Scene::Draw()
	{
		for (auto gameObject : m_gameObjects)
		{
			gameObject->Draw();
		}
	}
	GameObject* Scene::Find(const std::string& name)
	{
		for (auto gameObject : m_gameObjects)
		{
			if (gameObject->m_name == name)
			{
				return gameObject;
			}
		}

		return nullptr;

	}
	void Scene::AddGameObject(GameObject* gameObject)
	{
		m_gameObjects.push_back(gameObject);
		
	}
	
	void Scene::RemoveGameObject(GameObject* gameObject)
	{
		auto iter = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
		if (iter != m_gameObjects.end())
		{
			(*iter)->Destroy();
			delete (*iter);
			m_gameObjects.erase(iter);
		}

	}
	
	void Scene::RemoveAllGameObjects()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			gameObject->Destroy();
			delete gameObject;
		}

		m_gameObjects.clear();

	}
}
