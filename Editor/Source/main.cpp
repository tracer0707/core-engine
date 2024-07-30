#include <iostream>
#include <chrono>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include <System/DeviceContext.h>
#include <Shared/String.h>
#include <Shared/Path.h>
#include <Renderer/RendererGL4.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Components/Camera.h>
#include <Components/Transform.h>

#include "Editor/Editor.h"

Core::Object* object = nullptr;
Core::Camera* camera = nullptr;
Core::Transform* transform = nullptr;
Core::Scene* scene = nullptr;

int fpsInterval = 100;
int fpsTicks = 0;

int main(int argc, char* argv[])
{
    Core::DeviceContext* ctx = new Core::DeviceContext();
    if (ctx->createWindow("Core Engine", 1366, 768) != 0)
        return -1;

    scene = new Core::Scene();

    object = new Core::Object();
    transform = object->addComponent<Core::Transform*>();
    camera = object->addComponent<Core::Camera*>();

    transform->setPosition(glm::vec3(0, -5, -5));
    transform->setRotation(glm::vec3(-45, 0, 0));

    scene->setMainCamera(camera);

    Editor::Editor::init();
    Editor::Editor::setScene(scene);
    Editor::Editor::setCamera(camera);

    bool isRunning = true;

    while (isRunning)
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        ctx->update(isRunning);
        ctx->clear();

        scene->render();

        ctx->renderUiBegin();
        Editor::Editor::renderUI();
        ctx->renderUiEnd();

        ctx->swapWindow();

        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = end_time - start_time;
        auto fps = std::chrono::milliseconds(1000) / elapsed;

        if (fpsTicks < fpsInterval)
        {
            ++fpsTicks;
        }
        else
        {
            fpsTicks = 0;
            ctx->setWindowTitle(("Core Engine: " + std::to_string(fps) + "fps").c_str());
        }
    }

    Editor::Editor::free();

    ctx->destroyWindow();

    return 0;
}