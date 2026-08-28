#pragma once

#include "Inspector.h"

namespace Core
{
	class Transform;
} // namespace Core

namespace Editor
{
	class TransformInspector : public Inspector
	{
		private:
			Core::Transform* _transform = nullptr;

		public:
			TransformInspector(Core::Transform* transform, Core::EventHandler* eventHandler);
			virtual ~TransformInspector();

			virtual void build();
	};
} // namespace Editor