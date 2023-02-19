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

float points[] = {
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

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    Shader shader;
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
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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