#pragma once

#include "Window.h"

namespace Editor
{
	class WindowManager;
	class ProjectManagerWindow : public Window
	{
	private:
	public:
		ProjectManagerWindow(WindowManager* parent);
		virtual ~ProjectManagerWindow();
	};
}