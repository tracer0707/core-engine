#include "Editor.h"

#include <iostream>

#include <System/Time.h>
#include <Shared/String.h>
#include <Shared/Path.h>
#include <Renderer/Renderer.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Assets/RenderTexture.h>
#include <Assets/AssetManager.h>

#include "../Editor/Windows/WindowManager.h"
#include "../Editor/Windows/MainMenu.h"
#include "../Editor/Windows/SceneWindow.h"
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
    /* WINDOW */

    Editor::MainWindow::MainWindow() : Window("Project Manager", 1366, 768)
    {
        scene = new Core::Scene(_renderer);
        
        cameraObject = scene->createObject();
        camera = cameraObject->addComponent<Core::Camera*>();
        Core::Transform* cameraTransform = (Core::Transform*)cameraObject->findComponent<Core::Transform*>();

        renderTexture = _assetManager->createRenderTexture(512, 512);
        camera->setRenderTexture(renderTexture);

        scene->setMainCamera(camera);

        cameraTransform->setPosition(glm::vec3(0, 5, 5));
        cameraTransform->setRotation(glm::vec3(-10, 0, 0));

        windowManager = new WindowManager();
        windowManager->setTime(_time);
        windowManager->setRenderer(_renderer);
        windowManager->setAssetManager(_assetManager);
        windowManager->setInputManager(_inputManager);

        csgModifier = ModifierManager::singleton()->addModifier<CSGModifier*>();

        mainMenu = new MainMenu();
        windowManager->setMenuBar(mainMenu->getMenuBar());

        sceneWindow = windowManager->addWindow<SceneWindow*>();
        sceneWindow->setTime(_time);
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
    }

    Editor::MainWindow::~MainWindow()
    {
        ModifierManager::singleton()->destroy();

        delete windowManager;
        delete scene;
    }

    void Editor::MainWindow::update()
    {
        ModifierManager::singleton()->update();
    }

    void Editor::MainWindow::render()
    {
        //** Render scene begin **//
        camera->getRenderTexture()->bind();

        int viewportWidth = renderTexture->getWidth();
        int viewportHeight = renderTexture->getHeight();

        _renderer->setViewportSize(viewportWidth, viewportHeight);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.4f, 0.4f, 0.4f, 1.0f));

        Rendering::renderGrid(_renderer, _assetManager->getDefaultMaterial(), camera);
        ModifierManager::singleton()->render();
        scene->render();

        _renderer->bindFrameBuffer(nullptr);
        //** Render scene end **//

        //** Render UI begin **//
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        windowManager->update(_width, _height);
        _renderer->endUI();
        //** Render UI end **//

        setTitle(("Core Engine: " + std::to_string(_time->getFramesPerSecond()) + "fps").c_str());
    }

    /* EDITOR */

    void Editor::init()
    {
        wnd = new MainWindow();
        addWindow(wnd);
    }

    void Editor::destroy()
    {
        wnd = nullptr;
    }
}