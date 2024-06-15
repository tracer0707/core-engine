#include "DeviceContext.h"

#include <sdl/SDL.h>
#include <GL/glew.h>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "../Renderer/Renderer.h"
#include "../System/EventHandler.h"

//#ifdef _WIN32
//#include <windows.h>
//#endif

namespace Core
{
    int DeviceContext::createWindow(UString title, int width, int height)
    {
        if (window != nullptr)
            return 0;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

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

        return 0;
    }

    void DeviceContext::update(bool& isRunning)
    {
        SDL_Event event;
        int w, h;

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

            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        EventHandler::singleton()->processEvents();
    }

    void DeviceContext::render()
    {
        oldtime = SDL_GetTicks();

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (renderFunction != nullptr)
            renderFunction();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();

        if (renderUIFunction != nullptr)
            renderUIFunction();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_MakeCurrent((SDL_Window*)window, (SDL_GLContext)rendererContext);
        SDL_GL_SwapWindow((SDL_Window*)window);

        newtime = SDL_GetTicks() - oldtime;
        fps = (newtime > 0) ? 1000.0f / newtime : 0.0f;

        SDL_SetWindowTitle((SDL_Window*)window, ("GPU Renderer: " + std::to_string(fps) + "fps").c_str());
    }

    void DeviceContext::destroyWindow()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyWindow((SDL_Window*)window);

        renderFunction = nullptr;
        renderUIFunction = nullptr;
        window = nullptr;
        rendererContext = nullptr;

        SDL_Quit();
    }
}