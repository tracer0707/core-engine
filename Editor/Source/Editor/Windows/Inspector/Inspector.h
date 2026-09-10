#pragma once

#include <functional>

#include "../../Controls/LinearLayout.h"

namespace Core
{
	class EventHandler;
	class ContentManager;
}

namespace Editor
{
	class InspectorWindow;

	class Inspector : public LinearLayout
	{
		protected:
			Core::EventHandler* _eventHandler = nullptr;
			Core::ContentManager* _contentManager = nullptr;

		public:
			Inspector(Core::EventHandler* eventHandler, Core::ContentManager* contentManager);
			virtual ~Inspector();

			virtual void build() = 0;
	};
} // namespace Editor