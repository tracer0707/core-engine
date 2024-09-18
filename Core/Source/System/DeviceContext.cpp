#include "DeviceContext.h"

#include <sdl/SDL.h>
#include <GL/glew.h>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "../Renderer/Renderer.h"
#include "../System/EventHandler.h"
#include "../System/InputManager.h"
#include "../System/Time.h"

namespace Core
{
    int DeviceContext::createWindow(UString title, int width, int height)
    {
        if (window != nullptr)
            return 0;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

        window = SDL_CreateWindow(ToStdString(title).c_str(), SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

        if (window == NULL) {
            return -1;
        }

        rendererContext = SDL_GL_CreateContext((SDL_Window*)window);

        SDL_GL_SetSwapInterval(1);
        SDL_GL_MakeCurrent((SDL_Window*)window, (SDL_GLContext)rendererContext);
        SDL_GL_SwapWindow((SDL_Window*)window);

        glewInit();

        Core::Renderer::init(this);
        Renderer::singleton()->setViewportSize(width, height);

        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window, (SDL_GLContext)rendererContext);
        ImGui_ImplOpenGL3_Init("#version 130");

        glEnable(GL_MULTISAMPLE);

        return 0;
    }

    void DeviceContext::setWindowTitle(UString title)
    {
        SDL_SetWindowTitle((SDL_Window*)window, ToStdString(title).c_str());
    }

    void DeviceContext::update(bool& isRunning)
    {
        SDL_Event event;
        int w, h;

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
                    SDL_GetWindowSize((SDL_Window*)window, &w, &h);
                    Core::Renderer::singleton()->setViewportSize(w, h);
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
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        InputManager::singleton()->updateMouse(window);
        EventHandler::singleton()->processEvents();
    }

    void DeviceContext::renderUiBegin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void DeviceContext::renderUiEnd()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void DeviceContext::swapWindow()
    {
        InputManager::singleton()->reset();
        SDL_GL_MakeCurrent((SDL_Window*)window, (SDL_GLContext)rendererContext);
        SDL_GL_SwapWindow((SDL_Window*)window);

        Time::endTimer();
    }

    void DeviceContext::destroyWindow()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyWindow((SDL_Window*)window);

        window = nullptr;
        rendererContext = nullptr;

        SDL_Quit();
    }
}