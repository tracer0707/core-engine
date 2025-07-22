#pragma once

#include "Window.h"

namespace Editor
{
	class InspectorWindow : public Window
	{
	private:
	public:
		InspectorWindow(WindowManager* parent);
		virtual ~InspectorWindow() = default;
	};
}