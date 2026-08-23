#pragma once

#include "Inspector.h"

namespace Core
{
	class Transformable;
} // namespace Core

namespace Editor
{
	class TransformInspector : public Inspector
	{
		private:
			Core::Transformable* _transform = nullptr;

		public:
			TransformInspector(Core::Transformable* transform, Core::EventHandler* eventHandler);
			virtual ~TransformInspector();

			virtual void build();
	};
} // namespace Editor