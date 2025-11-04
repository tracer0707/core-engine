#include "Window.h"

#include <stdexcept>
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include "../Assets/AssetManager.h"
#include "../Renderer/Renderer.h"
#include "../System/Application.h"
#include "../System/EventHandler.h"
#include "../System/InputManager.h"
#include "../System/Time.h"

namespace Core
{
    Window::Window(Application* application, String title, int width, int height)
    {
        _width = width;
        _height = height;

        _ctx = SDL_CreateWindow(title.std_str().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

        if (_ctx == NULL)
        {
            throw std::runtime_error("Create window failed");
        }

        _application = application;
        _renderer = Renderer::init(_ctx);
        _assetManager = new AssetManager(_renderer);
        _time = new Time();
        _inputManager = new InputManager();
        _eventHandler = new EventHandler();
        _opened = true;

        _application->addWindow(this);
    }

    Window::~Window()
    {
        _renderer->makeCurrent();

        delete _assetManager;
        delete _time;
        delete _inputManager;
        delete _eventHandler;

        Renderer::destroy(_renderer);
        SDL_DestroyWindow((SDL_Window*)_ctx);

        _application = nullptr;
        _ctx = nullptr;
        _assetManager = nullptr;
        _renderer = nullptr;
        _time = nullptr;
        _inputManager = nullptr;
        _eventHandler = nullptr;
    }

    void Window::setTitle(String title)
    {
        SDL_SetWindowTitle((SDL_Window*)_ctx, title.std_str().c_str());
    }

    void Window::processEvents(void* event)
    {
        SDL_Event& evt = *((SDL_Event*)event);

        switch (evt.type)
        {
        case SDL_QUIT: {
            _opened = false;
            break;
        }

        case SDL_WINDOWEVENT: {
            const SDL_WindowEvent& wev = evt.window;
            switch (wev.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                SDL_GetWindowSize((SDL_Window*)_ctx, &_width, &_height);
                break;
            }
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_FOCUS_GAINED: {
                // TODO
                break;
            }

            case SDL_WINDOWEVENT_CLOSE: {
                _opened = false;

                if (_onClose != nullptr)
                {
                    _onClose();
                }

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
        _eventHandler->processEvents();

        update();
        render();

        _inputManager->reset();
        _renderer->swapBuffers();
        _time->endTimer();
    }

    void Window::close()
    {
        _opened = false;
    }
} // namespace Core