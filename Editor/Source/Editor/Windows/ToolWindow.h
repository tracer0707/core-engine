#pragma once

#include "Window.h"

namespace Editor
{
	class MenuBar;

	class ToolWindow : public Window
	{
	private:
		MenuBar* _menuBar = nullptr;

	public:
		ToolWindow();
		virtual ~ToolWindow();
	};
}