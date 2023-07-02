#include <sdl/SDL.h>
#include <GL/glew.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shared/String.h"
#include "Shared/Path.h"
#include "Renderer/RendererGL4.h"
#include "Assets/Shader.h"

#ifdef _WIN32
#include <windows.h>
#endif

real points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

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

    Assimp::Importer* importer = new Assimp::Importer();
    importer->SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f);
    importer->SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 60.0f);
    importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

    unsigned int importFlags =
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_GlobalScale |
        aiProcess_CalcTangentSpace |
        aiProcess_PopulateArmatureData;

    importFlags |= aiProcess_JoinIdenticalVertices;
    importFlags |= aiProcess_GenSmoothNormals | aiProcess_ForceGenNormals;

    UString modelPath = Core::Path::combine(Core::Path::getExePath(), "Test Project", "model.fbx");
    const aiScene* scene = importer->ReadFile(Core::String::toStdString(modelPath), importFlags);

    if (!scene)
    {
        std::cout << "Error loading model";
    }

    uint indices[] = { 0, 1, 2 };
    Core::Buffer buffer = Core::Renderer::singleton()->createBuffer(points, 9, indices, 3);

    Core::Shader shader;
    shader.loadFromString("#version 400\n"
        "in vec3 vp;"
        "void main() {"
        "  gl_Position = vec4(vp, 1.0);"
        "}",
        "#version 400\n"
        "out vec4 frag_colour;"
        "void main() {"
        "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
        "}");

    bool running = true;

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
                        //TODO
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
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();

        Core::Renderer::singleton()->bindBuffer(buffer);
        Core::Renderer::singleton()->drawBuffer(buffer);

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
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}