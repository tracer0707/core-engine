#pragma once

#include <glm/mat4x4.hpp>

#include "ComponentType.h"

namespace Core
{
	class Object;

	class Component
	{
			friend class Object;

		protected:
			Object* _owner = nullptr;

		public:
			Component(Object* owner);
			virtual ~Component();

			virtual ComponentType getComponentType() const = 0;

			virtual void update(float& dt) {}
			virtual void render(glm::mat4& view, glm::mat4& proj) {}

			Object* getOwner() { return _owner; }
	};
} // namespace Core