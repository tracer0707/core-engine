#pragma once

#include "../Config.h"

namespace Core
{
	class Object;
	class Camera;

	class Component
	{
		friend class Object;

	protected:
		Object* owner = nullptr;

	public:
		Component(Object* owner);
		virtual ~Component();

		virtual UInt32 getComponentType() = 0;

		virtual void update(float& dt);
		virtual void render(Camera* camera);

		Object* getOwner() { return owner; }
	};
}