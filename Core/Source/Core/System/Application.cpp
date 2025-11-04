#include "Application.h"

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include "Window.h"
#include "EventHandler.h"
#include "../Shared/Path.h"

namespace Core
{
    void Application::run()
    {
        SDL_SetMainReady();
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

        internalInit();
        internalLoop();
        internalDestroy();

        SDL_Quit();
    }

    void Application::stop(bool forceClose)
    {
        _isRunning = false;
        _forceClosed = forceClose;
    }

    void Application::internalInit()
    {
        _eventHandler = new EventHandler();

        init();

        if (!_windows.isEmpty())
        {
            _isRunning = true;
        }
    }

    void Application::internalLoop()
    {
        while (_isRunning)
        {
            List<Window*> windowsToClose;

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                for (Window* wnd : _windows)
                {
                    void* wndCtx = SDL_GetWindowFromID(event.window.windowID);
                    if (wndCtx != wnd->_ctx) continue;

                    wnd->processEvents(&event);
                }
            }

            for (Window* wnd : _windows)
            {
                wnd->internalUpdate();
            }

            _eventHandler->processEvents();

            for (Window* wnd : _windows)
            {
                if (!wnd->_opened)
                {
                    windowsToClose.add(wnd);
                }
            }

            for (Window* wnd : windowsToClose)
            {
                if (_mainWindow == wnd)
                {
                    _isRunning = false;
                }

                removeWindow(wnd);
            }

            windowsToClose.clear();

            if (_windows.isEmpty())
            {
                _isRunning = false;
            }

            if (!_isRunning) break;
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

        delete _eventHandler;

        _eventHandler = nullptr;
        _mainWindow = nullptr;
    }

    void Application::addWindow(Window* value)
    {
        _windows.add(value);

        if (_mainWindow == nullptr)
        {
            _mainWindow = value;
        }
    }

    void Application::removeWindow(Window* value)
    {
        if (_mainWindow == value)
        {
            _mainWindow = nullptr;
        }

        _windows.remove(value);
        delete value;
    }

    String Application::getContentDir()
    {
        return Path::combine(_workingDir, _contentDir);
    }
} // namespace Core