#pragma once

#include "Window.h"

namespace Editor
{
	class MenuBar;

	class MenuWindow : public Window
	{
	private:
		MenuBar* _menuBar = nullptr;

	public:
		MenuWindow();
		virtual ~MenuWindow();
	};
}