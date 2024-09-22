#include <iostream>

#include <GL/glew.h>

#include <System/DeviceContext.h>
#include <System/Time.h>
#include <Shared/String.h>
#include <Shared/Path.h>
#include <Renderer/RendererGL4.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Components/Camera.h>
#include <Components/Transform.h>

#include "Editor/Windows/MainMenu.h"
#include "Editor/Windows/WindowManager.h"
#include "Editor/Windows/ObjectWindow.h"
#include "Editor/Windows/CSGObjectWindow.h"
#include "Editor/Windows/InspectorWindow.h"
#include "Editor/Windows/HierarchyWindow.h"
#include "Editor/Windows/AssetsWindow.h"
#include "Editor/Windows/GizmoWindow.h"
#include "Editor/Windows/CSGEditWindow.h"

#include "Editor/Modifiers/ModifierManager.h"
#include "Editor/Modifiers/CSGModifier.h"

#include "Editor/CameraController.h"
#include "Editor/Rendering.h"
#include "Editor/Gizmo.h"

Core::Object* object = nullptr;
Core::Camera* camera = nullptr;
Core::Transform* transform = nullptr;
Core::Scene* scene = nullptr;

Editor::MainMenu* mainMenu = nullptr;
Editor::ObjectWindow* objectWindow = nullptr;
Editor::CSGObjectWindow* csgObjectWindow = nullptr;
Editor::InspectorWindow* inspectorWindow = nullptr;
Editor::HierarchyWindow* hierarchyWindow = nullptr;
Editor::AssetsWindow* assetsWindow = nullptr;
Editor::GizmoWindow* gizmoWindow = nullptr;
Editor::CSGEditWindow* csgEditWindow = nullptr;

Editor::CSGModifier* csgModifier = nullptr;

int main(int argc, char* argv[])
{
    Core::DeviceContext* ctx = new Core::DeviceContext();
    if (ctx->createWindow("Core Engine", 1366, 768) != 0)
        return -1;

    object = new Core::Object();
    transform = object->addComponent<Core::Transform*>();
    camera = object->addComponent<Core::Camera*>();

    transform->setPosition(glm::vec3(0, 5, 5));
    transform->setRotation(glm::vec3(-10, 0, 0));

    scene = new Core::Scene();
    scene->setMainCamera(camera);

    Editor::CameraController::init(camera);

    csgModifier = new Editor::CSGModifier();
    Editor::ModifierManager::singleton()->addModifier(csgModifier);

    Editor::WindowManager::singleton()->init();

    mainMenu = new Editor::MainMenu();
    Editor::WindowManager::singleton()->setMenuBar(mainMenu->getMenuBar());

    inspectorWindow = new Editor::InspectorWindow();
    hierarchyWindow = new Editor::HierarchyWindow();
    assetsWindow = new Editor::AssetsWindow();
    
    gizmoWindow = new Editor::GizmoWindow();
    gizmoWindow->setHasTitle(false);
    gizmoWindow->setCanAcceptDocking(false);
    gizmoWindow->setCanDock(false);

    objectWindow = new Editor::ObjectWindow();
    objectWindow->setHasTitle(false);
    objectWindow->setCanAcceptDocking(false);
    objectWindow->setCanDock(false);

    csgObjectWindow = new Editor::CSGObjectWindow();
    csgObjectWindow->setHasTitle(false);
    csgObjectWindow->setCanAcceptDocking(false);
    csgObjectWindow->setCanDock(false);
    csgObjectWindow->setVisible(false);

    csgEditWindow = new Editor::CSGEditWindow();
    csgEditWindow->setHasTitle(false);
    csgEditWindow->setCanAcceptDocking(false);
    csgEditWindow->setCanDock(false);
    csgEditWindow->setVisible(false);

    Editor::WindowManager::singleton()->addWindow(objectWindow);
    Editor::WindowManager::singleton()->addWindow(csgObjectWindow);
    Editor::WindowManager::singleton()->addWindow(gizmoWindow);
    Editor::WindowManager::singleton()->addWindow(csgEditWindow);
    Editor::WindowManager::singleton()->addWindow(inspectorWindow);
    Editor::WindowManager::singleton()->addWindow(hierarchyWindow);
    Editor::WindowManager::singleton()->addWindow(assetsWindow);

    Editor::ModifierManager::singleton()->init(scene);

    Editor::WindowManager::singleton()->setOnDock([] {
        auto dockInspector = inspectorWindow->dock(Editor::DockDirection::Right, 0, 0.25);
        auto dockHierarchy = hierarchyWindow->dock(Editor::DockDirection::Right, dockInspector.area2, 0.2f);
        auto dockAssets = assetsWindow->dock(Editor::DockDirection::Down, dockHierarchy.area2, 0.3f);
    });

    bool isRunning = true;

    while (isRunning)
    {
        ctx->update(isRunning);
        Editor::CameraController::update();
        Editor::ModifierManager::singleton()->update();
        
        int width = Core::Renderer::singleton()->getWidth();
        int height = Core::Renderer::singleton()->getHeight();

        Core::Renderer::singleton()->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Editor::Rendering::renderGrid(camera);
        Editor::ModifierManager::singleton()->render();
        scene->render();

        ctx->renderUiBegin();
        Editor::WindowManager::singleton()->update(width, height);
        Editor::Gizmo::singleton()->update(camera);
        ctx->renderUiEnd();

        ctx->swapWindow();

        ctx->setWindowTitle(("Core Engine: " + std::to_string(Core::Time::getFramesPerSecond()) + "fps").c_str());
    }

    Editor::WindowManager::singleton()->destroy();
    delete scene;

    ctx->destroyWindow();

    return 0;
}