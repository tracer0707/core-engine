#pragma once

#include "Window.h"

namespace Editor
{
	class GizmoWindow : public Window
	{
	private:

	public:
		GizmoWindow(WindowManager* parent);
		virtual ~GizmoWindow();

		static const char* NAME;
	};
}