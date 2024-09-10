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

#include "Editor/Editor.h"

Core::Object* object = nullptr;
Core::Camera* camera = nullptr;
Core::Transform* transform = nullptr;
Core::Scene* scene = nullptr;

Editor::MainMenu* mainMenu = nullptr;
Editor::WindowManager* windowManager = nullptr;
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
    mainMenu = new Editor::MainMenu();

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

    windowManager->setOnDock([] {
        auto dockInspector = inspectorWindow->dock(Editor::DockDirection::Right, 0, 0.25);
        auto dockHierarchy = hierarchyWindow->dock(Editor::DockDirection::Right, dockInspector.area2, 0.2f);
        auto dockAssets = assetsWindow->dock(Editor::DockDirection::Down, dockHierarchy.area2, 0.3f);
    });

    windowManager->setMenuBar(mainMenu->getMenuBar());

    windowManager->addWindow(objectWindow);
    windowManager->addWindow(csgObjectWindow);
    windowManager->addWindow(gizmoWindow);
    windowManager->addWindow(csgEditWindow);
    windowManager->addWindow(inspectorWindow);
    windowManager->addWindow(hierarchyWindow);
    windowManager->addWindow(assetsWindow);

    csgModifier = new Editor::CSGModifier();
    csgModifier->addWindow(csgObjectWindow);
    csgModifier->addWindow(csgEditWindow);

    csgObjectWindow->setModifier(csgModifier);
    csgEditWindow->setModifier(csgModifier);

    Editor::ModifierManager::singleton()->addModifier(csgModifier);

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