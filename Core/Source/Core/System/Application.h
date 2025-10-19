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
		void internalInit();
		void internalLoop();
		void internalDestroy();

		void addWindow(Window* value);
		void removeWindow(Window* value);

	protected:
		bool _isRunning = false;
		bool _forceClosed = false;

		String _workingDir = "";
		String _contentDir = "Content";

		List<Window*> _windows;

		EventHandler* _eventHandler = nullptr;
		Window* _mainWindow = nullptr;

		virtual void init() {}
		virtual void destroy() {}

	public:
		Window* getMainWindow() { return _mainWindow; }
		void setMainWindow(Window* value) { _mainWindow = value; }

		EventHandler* getEventHandler() { return _eventHandler; }
		bool isForceClosed() { return _forceClosed; }

		String getWorkingDir() { return _workingDir; }
		void setWorkingDir(String value) { _workingDir = value; }

		String getContentDir();

		void run();
		void stop(bool forceClose);
	};
}