#include "DeviceContext.h"

#include <sdl/SDL.h>

#include "../Renderer/Renderer.h"
#include "../System/EventHandler.h"
#include "../System/InputManager.h"
#include "../System/Time.h"

namespace Core
{
    int DeviceContext::createWindow(UString title, int width, int height)
    {
        if (window != nullptr)
        {
            return -1;
        }

        windowWidth = width;
        windowHeight = height;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

        window = SDL_CreateWindow(ToStdString(title).c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

        if (window == NULL)
        {
            return -1;
        }

        Core::Renderer::init(this);

        return 0;
    }

    void DeviceContext::setWindowTitle(UString title)
    {
        SDL_SetWindowTitle((SDL_Window*)window, ToStdString(title).c_str());
    }

    void DeviceContext::updateWindow(bool& isRunning)
    {
        SDL_Event event;

        Time::beginTimer();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                isRunning = false;
            }
            break;

            case SDL_WINDOWEVENT:
            {
                const SDL_WindowEvent& wev = event.window;
                switch (wev.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    SDL_GetWindowSize((SDL_Window*)window, &windowWidth, &windowHeight);
                }
                break;
                case SDL_WINDOWEVENT_RESTORED:
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    //TODO
                }
                break;

                case SDL_WINDOWEVENT_CLOSE:
                    isRunning = false;
                }
                break;
            }
            break;
            }

            InputManager::singleton()->updateKeys(&event);
            Renderer::singleton()->processEvents(&event);
        }

        InputManager::singleton()->updateMouse(window);
        EventHandler::singleton()->processEvents();
    }

    void DeviceContext::swapWindow()
    {
        InputManager::singleton()->reset();
        Renderer::singleton()->swapBuffers();
        Time::endTimer();
    }

    void DeviceContext::destroyWindow()
    {
        Core::Renderer::destroy();

        SDL_DestroyWindow((SDL_Window*)window);

        window = nullptr;

        SDL_Quit();
    }
}