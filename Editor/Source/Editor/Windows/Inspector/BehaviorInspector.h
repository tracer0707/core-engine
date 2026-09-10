#pragma once

#include "Inspector.h"

namespace Core
{
	class Behavior;
	class ContentManager;
} // namespace Core

namespace Editor
{
	class BehaviorInspector : public Inspector
	{
		private:
			Core::Behavior* _behavior = nullptr;

		public:
			BehaviorInspector(Core::Behavior* behavior, Core::EventHandler* eventHandler, Core::ContentManager* contentManager);
			virtual ~BehaviorInspector();

			virtual void build();
	};
} // namespace Editor