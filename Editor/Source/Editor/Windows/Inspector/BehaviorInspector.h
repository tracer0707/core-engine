#pragma once

#include "Inspector.h"

namespace Core
{
	class Behavior;
} // namespace Core

namespace Editor
{
	class BehaviorInspector : public Inspector
	{
		private:
			Core::Behavior* _behavior = nullptr;

		public:
			BehaviorInspector(Core::Behavior* behavior, Core::EventHandler* eventHandler);
			virtual ~BehaviorInspector();

			virtual void build();
	};
} // namespace Editor