#pragma once

#include "Window.h"

namespace Editor
{
	class InspectorWindow : public Window
	{
	private:
	public:
		InspectorWindow(UString name);
		virtual ~InspectorWindow() = default;
	};
}