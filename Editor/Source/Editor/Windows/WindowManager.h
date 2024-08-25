#pragma once

#include <Shared/List.h>

namespace Editor
{
	class Window;

	class WindowManager
	{
	private:
		Core::List<Window*> _windows;

	public:
		WindowManager();
		~WindowManager();

		void addWindow(Window* value);

		void update(int width, int height);
	};
}