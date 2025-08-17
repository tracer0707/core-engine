#include "Window.h"

#include <stdexcept>
#include <SDL/SDL.h>

#include "../Assets/AssetManager.h"
#include "../Renderer/Renderer.h"
#include "../System/EventHandler.h"
#include "../System/InputManager.h"
#include "../System/Time.h"

namespace Core
{
    Window::Window(UString title, int width, int height)
    {
        _width = width;
        _height = height;

        _ctx = SDL_CreateWindow(ToStdString(title).c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            _width,
            _height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

        if (_ctx == NULL)
        {
            throw new std::runtime_error("Create window failed");
        }

        _renderer = Renderer::init(_ctx);
        _assetManager = new AssetManager(_renderer);
        _time = new Time();
        _inputManager = new InputManager();

        _opened = true;
    }

    Window::~Window()
    {
        _renderer->makeCurrent();
        delete _assetManager;
        delete _time;
        delete _inputManager;
        Renderer::destroy(_renderer);
        SDL_DestroyWindow((SDL_Window*)_ctx);
        _ctx = nullptr;
        _assetManager = nullptr;
        _renderer = nullptr;
        _time = nullptr;
        _inputManager = nullptr;
    }

    void Window::setTitle(UString title)
    {
        SDL_SetWindowTitle((SDL_Window*)_ctx, ToStdString(title).c_str());
    }

    void Window::processEvents(void* event)
    {
        SDL_Event& evt = *((SDL_Event*)event);

        switch (evt.type)
        {
        case SDL_QUIT:
        {
            _opened = false;
            break;
        }

        case SDL_WINDOWEVENT:
        {
            const SDL_WindowEvent& wev = evt.window;
            switch (wev.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                SDL_GetWindowSize((SDL_Window*)_ctx, &_width, &_height);
                break;
            }
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                //TODO
                break;
            }

            case SDL_WINDOWEVENT_CLOSE:
            {
                _opened = false;
                break;
            }

            default:
                break;
            }

            break;
        }

        default:
            break;
        }

        _inputManager->updateKeys(event);
        _renderer->processEvents(event);
    }

    void Window::internalUpdate()
    {
        _time->beginTimer();
        _renderer->makeCurrent();

        _inputManager->updateMouse(_ctx);
        EventHandler::singleton()->processEvents();

        update();
        render();

        _inputManager->reset();
        _renderer->swapBuffers();
        _time->endTimer();
    }
}