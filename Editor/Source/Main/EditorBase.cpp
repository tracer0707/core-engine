#include "EditorBase.h"

#include <iostream>

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

namespace Editor
{
    int EditorBase::run()
    {
        ctx = new Core::DeviceContext();

        bool result = init();
        if (result != 0)
        {
            return result;
        }

        loop();
        destroy();

        delete ctx;

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

        windowManager = new WindowManager();

        csgModifier = ModifierManager::singleton()->addModifier<CSGModifier*>();

        mainMenu = new MainMenu();
        windowManager->setMenuBar(mainMenu->getMenuBar());

        sceneWindow = windowManager->addWindow<SceneWindow*>();
        sceneWindow->setScene(scene);
        sceneWindow->setRenderTexture(renderTexture);

        inspectorWindow = windowManager->addWindow<InspectorWindow*>();
        hierarchyWindow = windowManager->addWindow<HierarchyWindow*>();
        assetsWindow = windowManager->addWindow<AssetsWindow*>();

        gizmoWindow = windowManager->addWindow<GizmoWindow*>();
        gizmoWindow->setHasTitle(false);
        gizmoWindow->setCanAcceptDocking(false);
        gizmoWindow->setCanDock(false);

        objectWindow = windowManager->addWindow<ObjectWindow*>();
        objectWindow->setHasTitle(false);
        objectWindow->setCanAcceptDocking(false);
        objectWindow->setCanDock(false);

        csgObjectWindow = windowManager->addWindow<CSGObjectWindow*>();
        csgObjectWindow->setHasTitle(false);
        csgObjectWindow->setCanAcceptDocking(false);
        csgObjectWindow->setCanDock(false);
        csgObjectWindow->setVisible(false);

        csgEditWindow = windowManager->addWindow<CSGEditWindow*>();
        csgEditWindow->setHasTitle(false);
        csgEditWindow->setCanAcceptDocking(false);
        csgEditWindow->setCanDock(false);
        csgEditWindow->setVisible(false);

        ModifierManager::singleton()->init(windowManager, scene);

        windowManager->setOnDock([this] {
            auto dockInspector = inspectorWindow->dock(DockDirection::Right, 0, 0.25f);
            auto dockHierarchy = hierarchyWindow->dock(DockDirection::Right, dockInspector.area2, 0.2f);
            auto dockAssets = assetsWindow->dock(DockDirection::Down, dockHierarchy.area2, 0.3f);
            auto dockScene = sceneWindow->dock(DockDirection::None, dockAssets.area2, 0.7f);
            });

        isRunning = true;

        return 0;
    }

    void EditorBase::loop()
    {
        while (isRunning)
        {
            ctx->updateWindow(isRunning);

            ModifierManager::singleton()->update();

            //** Render scene begin **//
            camera->getRenderTexture()->bind();

            int viewportWidth = renderTexture->getWidth();
            int viewportHeight = renderTexture->getHeight();

            Core::Renderer::current()->setViewportSize(viewportWidth, viewportHeight);
            Core::Renderer::current()->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.4f, 0.4f, 0.4f, 1.0f));

            Rendering::renderGrid(camera);
            ModifierManager::singleton()->render();
            scene->render();

            Core::Renderer::current()->bindFrameBuffer(nullptr);
            //** Render scene end **//

            //** Render UI begin **//
            int width = ctx->getWindowWidth();
            int height = ctx->getWindowHeight();

            Core::Renderer::current()->setViewportSize(width, height);
            Core::Renderer::current()->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

            Core::Renderer::current()->beginUI();
            windowManager->update(width, height);
            Core::Renderer::current()->endUI();
            //** Render UI end **//

            ctx->swapWindow();

            ctx->setWindowTitle(("Core Engine: " + std::to_string(Core::Time::getFramesPerSecond()) + "fps").c_str());
        }
    }

    void EditorBase::destroy()
    {
        ModifierManager::singleton()->destroy();
        
        delete windowManager;
        delete renderTexture;
        delete scene;

        ctx->destroyWindow();
    }
}