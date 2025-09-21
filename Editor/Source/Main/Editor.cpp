#include "Editor.h"

#include <iostream>
#include <filesystem>

#include <Core/System/Time.h>
#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Scene/Scene.h>
#include <Core/Scene/Object.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/Assets/RenderTexture.h>
#include <Core/Assets/AssetManager.h>

#include "../Editor/Font.h"
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

    Editor::MainWindow::MainWindow(Core::Application* app) : Window(app, "Project Manager", 1366, 768)
    {
        _scene = new Core::Scene(_renderer);
        
        _cameraObject = _scene->createObject();
        _camera = _cameraObject->addComponent<Core::Camera*>();
        Core::Transform* cameraTransform = (Core::Transform*)_cameraObject->findComponent<Core::Transform*>();

        _renderTexture = _assetManager->createRenderTexture(512, 512);
        _camera->setRenderTexture(_renderTexture);

        _scene->setMainCamera(_camera);

        cameraTransform->setPosition(glm::vec3(0, 5, 5));
        cameraTransform->setRotation(glm::vec3(-10, 0, 0));

        _windowManager = new WindowManager();
        _windowManager->setTime(_time);
        _windowManager->setRenderer(_renderer);
        _windowManager->setAssetManager(_assetManager);
        _windowManager->setInputManager(_inputManager);
        _windowManager->setEventHandler(_eventHandler);

        _csgModifier = ModifierManager::singleton()->addModifier<CSGModifier*>();

        _mainMenu = new MainMenu();
        _windowManager->setMenuBar(_mainMenu->getMenuBar());

        _sceneWindow = _windowManager->addWindow<SceneWindow*>();
        _sceneWindow->setTime(_time);
        _sceneWindow->setScene(_scene);
        _sceneWindow->setRenderTexture(_renderTexture);

        _inspectorWindow = _windowManager->addWindow<InspectorWindow*>();
        _hierarchyWindow = _windowManager->addWindow<HierarchyWindow*>();
        _assetsWindow = _windowManager->addWindow<AssetsWindow*>();

        _gizmoWindow = _windowManager->addWindow<GizmoWindow*>();
        _gizmoWindow->setHasTitle(false);
        _gizmoWindow->setCanAcceptDocking(false);
        _gizmoWindow->setCanDock(false);

        _objectWindow = _windowManager->addWindow<ObjectWindow*>();
        _objectWindow->setHasTitle(false);
        _objectWindow->setCanAcceptDocking(false);
        _objectWindow->setCanDock(false);

        _csgObjectWindow = _windowManager->addWindow<CSGObjectWindow*>();
        _csgObjectWindow->setHasTitle(false);
        _csgObjectWindow->setCanAcceptDocking(false);
        _csgObjectWindow->setCanDock(false);
        _csgObjectWindow->setVisible(false);

        _csgEditWindow = _windowManager->addWindow<CSGEditWindow*>();
        _csgEditWindow->setHasTitle(false);
        _csgEditWindow->setCanAcceptDocking(false);
        _csgEditWindow->setCanDock(false);
        _csgEditWindow->setVisible(false);

        ModifierManager::singleton()->init(_windowManager, _scene);

        _windowManager->setOnDock([this] {
            auto dockInspector = _inspectorWindow->dock(DockDirection::Right, 0, 0.25f);
            auto dockHierarchy = _hierarchyWindow->dock(DockDirection::Right, dockInspector.area2, 0.2f);
            auto dockAssets = _assetsWindow->dock(DockDirection::Down, dockHierarchy.area2, 0.3f);
            auto dockScene = _sceneWindow->dock(DockDirection::None, dockAssets.area2, 0.7f);
        });
    }

    Editor::MainWindow::~MainWindow()
    {
        ModifierManager::singleton()->destroy();

        delete _windowManager;
        delete _scene;
    }

    void Editor::MainWindow::update()
    {
        ModifierManager::singleton()->update();
    }

    void Editor::MainWindow::render()
    {
        //** Render scene begin **//
        _camera->getRenderTexture()->bind();

        int viewportWidth = _renderTexture->getWidth();
        int viewportHeight = _renderTexture->getHeight();

        _renderer->setViewportSize(viewportWidth, viewportHeight);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.4f, 0.4f, 0.4f, 1.0f));

        Rendering::renderGrid(_renderer, _assetManager->getDefaultMaterial(), _camera);
        ModifierManager::singleton()->render();
        _scene->render();

        _renderer->bindFrameBuffer(nullptr);
        //** Render scene end **//

        //** Render UI begin **//
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        _windowManager->update(_width, _height);
        _renderer->endUI();
        //** Render UI end **//

        setTitle(("Core Engine: " + std::to_string(_time->getFramesPerSecond()) + "fps").c_str());
    }

    /* EDITOR */

    void Editor::init()
    {
        _wnd = new MainWindow(this);

        _mainFont = new Font(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();
    }

    void Editor::destroy()
    {
        delete _mainFont;

        _mainFont = nullptr;
        _wnd = nullptr;
    }
}