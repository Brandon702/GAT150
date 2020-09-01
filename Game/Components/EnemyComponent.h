#pragma once
#include "Components/Component.h"

namespace nc
{
	class EnemyComponent : public Component
	{
	public:
		virtual void Create(void* data = nullptr) override;
		virtual void Destroy() override;
		virtual Object* Clone() const override { return new EnemyComponent{ *this }; }
		virtual void Update() override;
		void Read(const rapidjson::Value& value) override;

	protected:
		float m_speed = 0;
	};
}