#include <iostream>

#include <sdl/SDL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/mat4x4.hpp>

#include "System/DeviceContext.h"
#include "Shared/String.h"
#include "Shared/Path.h"
#include "Renderer/RendererGL4.h"

#include "Scene/Scene.h"
#include "Components/Camera.h"
#include "Editor/Editor.h"

Core::Camera* camera = nullptr;
Core::Scene* scene = nullptr;

void renderScene();
void renderUI();

int main(int argc, char* argv[])
{
    Core::DeviceContext* ctx = new Core::DeviceContext();
    if (ctx->createWindow("Core", 1366, 768) != 0)
        return -1;

    ctx->setRenderFunction(renderScene);
    ctx->setRenderUIFunction(renderUI);

    camera = new Core::Camera();
    scene = new Core::Scene();

    Editor::Editor::init();
    Editor::Editor::setScene(scene);
    Editor::Editor::setCamera(camera);
    
    bool isRunning = true;

    while (isRunning)
    {
        ctx->update(isRunning);
        ctx->render();
    }

    Editor::Editor::free();

    ctx->destroyWindow();

    return 0;
}

void renderScene()
{
    scene->render(camera);
}

void renderUI()
{
    Editor::Editor::renderUI();
}