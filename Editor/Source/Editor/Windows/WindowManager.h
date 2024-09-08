#pragma once

#include <functional>

#include <Shared/List.h>

namespace Editor
{
	class Window;
	class MenuBar;

	class WindowManager
	{
	private:
		Core::List<Window*> _windows;

		MenuBar* _menuBar = nullptr;

		std::function<void()> _onDock = nullptr;

	public:
		WindowManager();
		~WindowManager();

		void addWindow(Window* value);
		void setOnDock(std::function<void()> value) { _onDock = value; }

		void setMenuBar(MenuBar* value) { _menuBar = value; }

		void update(int width, int height);
	};
}