#include "EditorBase.h"

#include <iostream>

//#include <GL/glew.h>

#include <System/DeviceContext.h>
#include <System/Time.h>
#include <Shared/String.h>
#include <Shared/Path.h>
#include <Renderer/Renderer.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Assets/RenderTexture.h>

#include "../Editor/Windows/MainMenu.h"
#include "../Editor/Windows/SceneWindow.h"
#include "../Editor/Windows/WindowManager.h"
#include "../Editor/Windows/ObjectWindow.h"
#include "../Editor/Windows/CSGObjectWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/Windows/AssetsWindow.h"
#include "../Editor/Windows/GizmoWindow.h"
#include "../Editor/Windows/CSGEditWindow.h"

#include "../Editor/Modifiers/ModifierManager.h"
#include "../Editor/Modifiers/CSGModifier.h"

#include "../Editor/CameraController.h"
#include "../Editor/Rendering.h"

int EditorBase::run(Core::DeviceContext* context)
{
    ctx = context;
    
    bool result = init();
    if (result != 0)
    {
        return result;
    }

    loop();
    destroy();

    return 0;
}

int EditorBase::init()
{
    if (ctx->createWindow("Core Engine", 1366, 768) != 0)
        return -1;

    object = new Core::Object();
    transform = object->addComponent<Core::Transform*>();
    camera = object->addComponent<Core::Camera*>();

    renderTexture = new Core::RenderTexture(512, 512);
    camera->setRenderTexture(renderTexture);

    transform->setPosition(glm::vec3(0, 5, 5));
    transform->setRotation(glm::vec3(-10, 0, 0));

    scene = new Core::Scene();
    scene->setMainCamera(camera);

    csgModifier = new Editor::CSGModifier();
    Editor::ModifierManager::singleton()->addModifier(csgModifier);

    Editor::WindowManager::singleton()->init();

    mainMenu = new Editor::MainMenu();
    Editor::WindowManager::singleton()->setMenuBar(mainMenu->getMenuBar());

    sceneWindow = new Editor::SceneWindow(scene, renderTexture);

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

    Editor::WindowManager::singleton()->addWindow(sceneWindow);
    Editor::WindowManager::singleton()->addWindow(objectWindow);
    Editor::WindowManager::singleton()->addWindow(csgObjectWindow);
    Editor::WindowManager::singleton()->addWindow(gizmoWindow);
    Editor::WindowManager::singleton()->addWindow(csgEditWindow);
    Editor::WindowManager::singleton()->addWindow(inspectorWindow);
    Editor::WindowManager::singleton()->addWindow(hierarchyWindow);
    Editor::WindowManager::singleton()->addWindow(assetsWindow);

    Editor::ModifierManager::singleton()->init(scene);

    Editor::WindowManager::singleton()->setOnDock([this] {
        auto dockInspector = inspectorWindow->dock(Editor::DockDirection::Right, 0, 0.25f);
        auto dockHierarchy = hierarchyWindow->dock(Editor::DockDirection::Right, dockInspector.area2, 0.2f);
        auto dockAssets = assetsWindow->dock(Editor::DockDirection::Down, dockHierarchy.area2, 0.3f);
        auto dockScene = sceneWindow->dock(Editor::DockDirection::None, dockAssets.area2, 0.7f);
        });

    isRunning = true;

    return 0;
}

void EditorBase::loop()
{
    while (isRunning)
    {
        ctx->updateWindow(isRunning);

        Editor::ModifierManager::singleton()->update();

        //** Render scene begin **//
        camera->getRenderTexture()->bind();

        int viewportWidth = renderTexture->getWidth();
        int viewportHeight = renderTexture->getHeight();

        Core::Renderer::singleton()->setViewportSize(viewportWidth, viewportHeight);
        Core::Renderer::singleton()->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.4f, 0.4f, 0.4f, 1.0f));

        Editor::Rendering::renderGrid(camera);
        Editor::ModifierManager::singleton()->render();
        scene->render();

        Core::Renderer::singleton()->bindFrameBuffer(nullptr);
        //** Render scene end **//

        //** Render UI begin **//
        int width = ctx->getWindowWidth();
        int height = ctx->getWindowHeight();

        Core::Renderer::singleton()->setViewportSize(width, height);
        Core::Renderer::singleton()->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        Core::Renderer::singleton()->beginUI();
        Editor::WindowManager::singleton()->update(width, height);
        Core::Renderer::singleton()->endUI();
        //** Render UI end **//

        ctx->swapWindow();

        ctx->setWindowTitle(("Core Engine: " + std::to_string(Core::Time::getFramesPerSecond()) + "fps").c_str());
    }
}

void EditorBase::destroy()
{
    Editor::ModifierManager::singleton()->destroy();
    Editor::WindowManager::singleton()->destroy();

    delete renderTexture;
    delete scene;

    ctx->destroyWindow();
}
