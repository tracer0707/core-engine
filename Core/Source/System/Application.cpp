#include "Application.h"

#include <SDL/SDL.h>

#include "Window.h"

namespace Core
{
	void Application::run()
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

		internalInit();
		internalLoop();
		internalDestroy();

		SDL_Quit();
	}

	void Application::internalInit()
	{
		init();
		isRunning = true;
	}

	void Application::internalLoop()
	{
		while (isRunning)
		{
			for (Window* wnd : _windows)
			{
				wnd->internalUpdate(isRunning);
			}
		}
	}

	void Application::internalDestroy()
	{
		destroy();

		for (Window* wnd : _windows)
		{
			delete wnd;
		}

		_windows.clear();
	}

	void Application::addWindow(Window* value)
	{
		_windows.add(value);
	}

	void Application::removeWindow(Window* value)
	{
		_windows.remove(value);
	}
}