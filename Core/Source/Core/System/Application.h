#pragma once

#include "../Shared/List.h"

#include "../Shared/Path.h"

typedef union SDL_Event;

namespace Core
{
	class Window;
	class EventHandler;

	class Application
	{
			friend class Window;
#ifdef _WIN32
			friend int ResizingEventWatcher(void* data, SDL_Event* event);
#endif

		private:
			void internalInit();
			void internalLoop();
			void internalDestroy();

			void addWindow(Window* value);
			void removeWindow(Window* value);

#ifdef _WIN32
			void updateWindowByEvent(void* event);
#endif

		protected:
			bool _isRunning = false;
			bool _forceClosed = false;

			fs::path _rootPath;
			fs::path _contentPath = "Content";

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

			fs::path getRootPath() const { return _rootPath; }
			void setRootPath(const fs::path& value) { _rootPath = value; }

			fs::path getContentPath() const;

			void run();
			void stop(bool forceClose);
	};
} // namespace Core