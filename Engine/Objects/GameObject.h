#pragma once
#include "Object.h"

namespace nc
{
	class GameObject : public Object
	{
		virtual void Create(void* data = nullptr) override;
		virtual void Destroy() override;
	};
}