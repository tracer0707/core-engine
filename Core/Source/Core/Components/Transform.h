#pragma once

#include "Component.h"

#include "../Shared/String.h"
#include "../Interface/Transformable.h"

namespace Core
{
	class Object;

	class Transform: public Transformable, public Component
	{
	public:
		Transform(Object* owner);
		virtual ~Transform();

		virtual unsigned int getComponentType();
	};
}