#pragma once

#include "../Shared/List.h"
#include "../Shared/String.h"

namespace Core
{
	class Window;

	class Application
	{
	private:
		bool isRunning = false;
		List<Window*> _windows;

		void internalInit();
		void internalLoop();
		void internalDestroy();

	protected:
		virtual void init() {}
		virtual void destroy() {}

	public:
		void addWindow(Window* value);
		void removeWindow(Window* value);
		void run();
	};
}