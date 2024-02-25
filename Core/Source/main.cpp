#include <sdl/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shared/String.h"
#include "Shared/Path.h"
#include "Renderer/RendererGL4.h"
#include "Assets/Mesh.h"
#include "Assets/Material.h"

#include "Components/Camera.h"
#include "Math/Mathf.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "Classes/imgui_impl_sdl2.h"
#include "Classes/imgui_impl_opengl3.h"

#include <glm/mat4x4.hpp>

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

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 130");

    Core::Camera* camera = new Core::Camera();
    camera->setPosition(glm::vec3(0, 0.15f, -1.5f));

    Core::Mesh* mesh = Core::Mesh::loadFromFile("D:/Dev/C++/core-engine/x64/Release/Test Project/model.fbx");

    bool running = true;

    glm::quat rotation = glm::identity<glm::quat>();
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1.0f);

    float yRot = 0.0f;

    int oldtime = 0;
    int newtime = 0;
    int fps = 0;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    SDL_Event event;
    while (running)
    {
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

        oldtime = SDL_GetTicks();

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::identity<glm::mat4x4>();
        glm::mat4 rotMat = glm::mat4_cast(rotation);

        glm::vec3 pos = glm::inverse(rotMat) * (glm::vec4(position, 1.0f));

        model = glm::translate(model, pos);
        model = rotMat * model;
        model = glm::scale(model, scale);

        yRot += 1.0f;
        rotation = Core::Mathf::toQuaternion(glm::vec3(90.0f, 0, 0)) * Core::Mathf::toQuaternion(glm::vec3(0, 0, yRot));

        for (int i = 0; i < mesh->getSubMeshesCount(); ++i)
        {
            Core::SubMesh* subMesh = mesh->getSubMesh(i);
            Core::Material* material = subMesh->getMaterial();

            glm::mat4 view = camera->getViewMatrix();
            glm::mat4 proj = camera->getProjectionMatrix((float)w / (float)h);

            Core::Renderer::singleton()->bindBuffer(subMesh->getVertexBuffer());
            
            if (material != nullptr)
                material->bind();

            Core::Renderer::singleton()->drawBuffer(subMesh->getVertexBuffer(), view, proj, model);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        bool dwin = true;
        ImGui::ShowDemoWindow(&dwin);

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}