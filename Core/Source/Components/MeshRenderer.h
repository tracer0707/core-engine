#pragma once

#include "Component.h"

namespace Core
{
	class Object;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(Object* owner);
		virtual ~MeshRenderer();

		virtual UInt32 getComponentType();

		void render();
	};
}