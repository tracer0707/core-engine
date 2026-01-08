#pragma once

#include "Window.h"

namespace Editor
{
	class Inspector;

	class InspectorWindow : public Window
	{
		private:
			Inspector* _currentInspector = nullptr;

		public:
			InspectorWindow(WindowManager* parent);
			virtual ~InspectorWindow() = default;

			void setInspector(Inspector* value);
	};
} // namespace Editor