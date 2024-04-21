#include <sdl/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <glm/mat4x4.hpp>

#include "Shared/String.h"
#include "Shared/Path.h"
#include "Renderer/RendererGL4.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "Scene/Scene.h"
#include "Components/Camera.h"
#include "Editor/Editor.h"
#include "System/EventHandler.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_Window* window = SDL_CreateWindow("Core", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 1366, 768,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (window == NULL) {
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext((SDL_Window*)window);

    SDL_GL_SetSwapInterval(1);
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SwapWindow(window);

    glewInit();

    Core::Renderer::init();

    Core::Camera* camera = new Core::Camera();
    Core::Scene* scene = new Core::Scene();
    Editor::Editor::init();
    Editor::Editor::setScene(scene);

    scene->setCamera(camera);

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool running = true;

    int oldtime = 0;
    int newtime = 0;
    int fps = 0;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    Core::Renderer::singleton()->setViewportSize(w, h);

    SDL_Event event;
    while (running)
    {
        oldtime = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = false;
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
                        SDL_GetWindowSize(window, &w, &h);
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
                        running = false;
                    }
                    break;
                }
                break;
            }

            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        Core::EventHandler::singleton()->processEvents();

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        
        ImGui::NewFrame();
        
        Editor::Editor::renderUI();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /////

        if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS))
        {
#ifdef _WIN32
            Sleep(200);
#else
            usleep(200 * 1000);
#endif
        }

        SDL_GL_MakeCurrent(window, context);
        SDL_GL_SwapWindow(window);

        newtime = SDL_GetTicks() - oldtime;
        fps = (newtime > 0) ? 1000.0f / newtime : 0.0f;

        SDL_SetWindowTitle(window, ("GPU Renderer: " + std::to_string(fps) + "fps").c_str());
    }

    Editor::Editor::free();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}