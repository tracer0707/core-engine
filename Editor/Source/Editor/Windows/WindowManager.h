#pragma once

#include <functional>

#include <Shared/List.h>

namespace Editor
{
	class Window;

	class WindowManager
	{
	private:
		Core::List<Window*> _windows;

		std::function<void()> _onDock = nullptr;

	public:
		WindowManager();
		~WindowManager();

		void addWindow(Window* value);
		void setOnDock(std::function<void()> value) { _onDock = value; }

		void update(int width, int height);
	};
}