#pragma once

#include "../Shared/List.h"
#include "../Shared/String.h"

namespace Core
{
	class Window;
	class EventHandler;

	class Application
	{
		friend class Window;

	private:
		bool _isRunning = false;
		List<Window*> _windows;

		EventHandler* _eventHandler = nullptr;
		Window* _mainWindow = nullptr;

		void internalInit();
		void internalLoop();
		void internalDestroy();

		void addWindow(Window* value);
		void removeWindow(Window* value);

	protected:
		virtual void init() {}
		virtual void destroy() {}

	public:
		Window* getMainWindow() { return _mainWindow; }
		void setMainWindow(Window* value) { _mainWindow = value; }
		EventHandler* getEventHandler() { return _eventHandler; }
		void run();
	};
}