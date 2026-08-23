#pragma once

#include <functional>

#include "../../Controls/LinearLayout.h"

namespace Core
{
	class EventHandler;
}

namespace Editor
{
	class InspectorWindow;

	class Inspector : public LinearLayout
	{
		protected:
			Core::EventHandler* _eventHandler = nullptr;

		public:
			Inspector(Core::EventHandler* eventHandler);
			virtual ~Inspector();

			virtual void build() = 0;
	};
} // namespace Editor