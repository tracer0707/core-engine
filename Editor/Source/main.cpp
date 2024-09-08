#include <iostream>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include <System/DeviceContext.h>
#include <System/Time.h>
#include <Shared/String.h>
#include <Shared/Path.h>
#include <Renderer/RendererGL4.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Components/Camera.h>
#include <Components/Transform.h>

#include "Editor/Windows/WindowManager.h"
#include "Editor/Windows/ToolWindow.h"
#include "Editor/Windows/InspectorWindow.h"
#include "Editor/Windows/HierarchyWindow.h"
#include "Editor/Editor.h"

Core::Object* object = nullptr;
Core::Camera* camera = nullptr;
Core::Transform* transform = nullptr;
Core::Scene* scene = nullptr;

Editor::WindowManager* windowManager = nullptr;
Editor::ToolWindow* toolWindow = nullptr;
Editor::InspectorWindow* inspectorWindow = nullptr;
Editor::HierarchyWindow* hierarchyWindow = nullptr;

int main(int argc, char* argv[])
{
    Core::DeviceContext* ctx = new Core::DeviceContext();
    if (ctx->createWindow("Core Engine", 1366, 768) != 0)
        return -1;

    scene = new Core::Scene();

    object = new Core::Object();
    transform = object->addComponent<Core::Transform*>();
    camera = object->addComponent<Core::Camera*>();

    transform->setPosition(glm::vec3(0, 5, 5));
    transform->setRotation(glm::vec3(-10, 0, 0));

    scene->setMainCamera(camera);

    Editor::Editor::init();
    Editor::Editor::setScene(scene);
    Editor::Editor::setCamera(camera);

    windowManager = new Editor::WindowManager();
    
    toolWindow = new Editor::ToolWindow();
    inspectorWindow = new Editor::InspectorWindow();
    hierarchyWindow = new Editor::HierarchyWindow();

    toolWindow->setHasTitle(false);
    toolWindow->setCanAcceptDocking(false);
    toolWindow->setCanDock(false);

    windowManager->setOnDock([] {
        auto dockInspector = inspectorWindow->dock(Editor::DockDirection::Right, 0, 0.25);
        auto dockHierarchy = hierarchyWindow->dock(Editor::DockDirection::Right, dockInspector.area2, 0.2f);
    });

    windowManager->addWindow(toolWindow);
    windowManager->addWindow(inspectorWindow);
    windowManager->addWindow(hierarchyWindow);

    bool isRunning = true;

    while (isRunning)
    {
        ctx->update(isRunning);
        Editor::Editor::update();
        
        int width = Core::Renderer::singleton()->getWidth();
        int height = Core::Renderer::singleton()->getHeight();

        Core::Renderer::singleton()->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Editor::Editor::render();
        scene->render();

        ctx->renderUiBegin();
        windowManager->update(width, height);
        ctx->renderUiEnd();

        ctx->swapWindow();

        ctx->setWindowTitle(("Core Engine: " + std::to_string(Core::Time::getFramesPerSecond()) + "fps").c_str());
    }

    Editor::Editor::free();

    delete scene;
    delete windowManager;

    ctx->destroyWindow();

    return 0;
}