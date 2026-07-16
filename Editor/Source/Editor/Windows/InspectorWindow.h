#pragma once

#include "Window.h"

namespace Editor
{
	class Inspector;

	class InspectorWindow : public Window
	{
		public:
			InspectorWindow(WindowManager* parent);
			virtual ~InspectorWindow();

			void rebuild();
	};
} // namespace Editor