#pragma once

#include <functional>

#include <Shared/List.h>
#include <Shared/String.h>

namespace Editor
{
	class Window;
	class MenuBar;

	class WindowManager
	{
	private:
		static WindowManager _singleton;

		Core::List<Window*> _windows;

		MenuBar* _menuBar = nullptr;

		std::function<void()> _onDock = nullptr;

	public:
		WindowManager();
		~WindowManager();

		static WindowManager* singleton() { return &_singleton; }

		void init();
		void destroy();

		void addWindow(Window* value);
		Window* getWindow(UString name);
		void setOnDock(std::function<void()> value) { _onDock = value; }

		bool isMouseCaptured();

		void setMenuBar(MenuBar* value) { _menuBar = value; }

		void update(int width, int height);
	};
}