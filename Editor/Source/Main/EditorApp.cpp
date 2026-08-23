#include "EditorApp.h"

#include <iostream>
#include <filesystem>
#include <imgui.h>

#include <Core/System/Time.h>
#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Scene/Scene.h>
#include <Core/Scene/Object.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/Content/RenderTexture.h>
#include <Core/Content/ContentManager.h>

#include "../Editor/Font.h"
#include "../Editor/Windows/WindowManager.h"
#include "../Editor/Windows/MainMenu.h"
#include "../Editor/Windows/SceneWindow.h"
#include "../Editor/Windows/ObjectWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/Windows/ContentWindow.h"
#include "../Editor/Windows/ToolWindow.h"
#include "../Editor/Windows/ContentImportWindow.h"
#include "../Editor/CameraController.h"
#include "../Editor/Rendering.h"
#include "../Editor/Gizmo.h"
#include "../Editor/ObjectPicker.h"

#include "../../Dependencies/ImGuizmo/ImGuizmo.h"

#include "../Editor/GizmoRenderer.h"

#include "../Shared/IconsForkAwesome.h"

namespace Editor
{
	/* WINDOW */

	EditorApp::MainWindow::MainWindow(EditorApp* app) : Window(app, "Core Editor", 1366, 768)
	{
		_scene = new Core::Scene(_renderer, _time);

		_cameraObject = _scene->createObject();
		_camera = _cameraObject->addComponent<Core::Camera*>();
		Core::Transform* cameraTransform = (Core::Transform*)_cameraObject->findComponent<Core::Transform*>();

		_renderTexture = _contentManager->createRenderTexture(512, 512);
		_camera->setRenderTexture(_renderTexture);

		_scene->setMainCamera(_camera);

		cameraTransform->setPosition(glm::vec3(0.0f, 5.0f, 5.0f));
		cameraTransform->setRotation(glm::vec3(-10.0f, 0, 0));

		_gridBuffer = _renderer->createBuffer(nullptr, 2048, nullptr, 0);

		_windowManager = new WindowManager(app);
		_windowManager->setTime(_time);
		_windowManager->setRenderer(_renderer);
		_windowManager->setContentManager(_contentManager);
		_windowManager->setInputManager(_inputManager);
		_windowManager->setEventHandler(_eventHandler);

		_mainMenu = new MainMenu();
		_windowManager->setMenuBar(_mainMenu->getMenuBar());

		_sceneWindow = _windowManager->addWindow<SceneWindow*>();
		_sceneWindow->setTime(_time);
		_sceneWindow->setScene(_scene);
		_sceneWindow->setRenderTexture(_renderTexture);

		_inspectorWindow = _windowManager->addWindow<InspectorWindow*>();
		_hierarchyWindow = _windowManager->addWindow<HierarchyWindow*>();
		_contentWindow = _windowManager->addWindow<ContentWindow*>();
		_contentImportWindow = _windowManager->addWindow<ContentImportWindow*>();
		_contentImportWindow->setVisible(false);

		_contentWindow->setContentDir(app->getContentPath());

		_toolWindow = _windowManager->addWindow<ToolWindow*>();
		_toolWindow->setCanClose(false);
		_toolWindow->setHasDockTitle(false);
		_toolWindow->setCanAcceptDocking(false);

		_objectWindow = _windowManager->addWindow<ObjectWindow*>();
		_objectWindow->setCanClose(false);
		_objectWindow->setHasDockTitle(false);
		_objectWindow->setCanAcceptDocking(false);

		_windowManager->setOnDock([this] {
			auto dockTools = _toolWindow->dock(DockDirection::Up, 0, 0.053f);
			auto dockInspector = _inspectorWindow->dock(DockDirection::Right, dockTools.area2, 0.25f);
			auto dockHierarchy = _hierarchyWindow->dock(DockDirection::Right, dockInspector.area2, 0.2f);
			auto dockContent = _contentWindow->dock(DockDirection::Down, dockHierarchy.area2, 0.3f);
			auto dockObjects = _objectWindow->dock(DockDirection::Left, dockContent.area2, 0.051f);
			auto dockScene = _sceneWindow->dock(DockDirection::None, dockObjects.area2, 0.7f);
		});

		_windowManager->initWindows();

		CameraController::init(_inputManager, _time, _camera);
		Gizmo::singleton()->init(_inputManager);
		ObjectPicker::singleton()->init(_windowManager, _scene, _camera);

		_gizmoRenderer = new GizmoRenderer(_renderer, _scene);
	}

	EditorApp::MainWindow::~MainWindow()
	{
		ObjectPicker::singleton()->destroy();

		_renderer->deleteBuffer(_gridBuffer);

		delete _gizmoRenderer;
		delete _windowManager;
		delete _scene;

		_gizmoRenderer = nullptr;
		_gridBuffer = nullptr;
		_windowManager = nullptr;
		_scene = nullptr;
	}

	void EditorApp::MainWindow::update()
	{
		//** Render scene begin **//
		_camera->getRenderTexture()->bind();

		int viewportWidth = _renderTexture->getWidth();
		int viewportHeight = _renderTexture->getHeight();

		_renderer->setViewportSize(viewportWidth, viewportHeight);
		_renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.4f, 0.4f, 0.4f, 1.0f));

		Rendering::renderGrid(_renderer, _gridBuffer, _camera);
		_scene->render();
		_gizmoRenderer->renderGizmo();

		_renderer->bindFrameBuffer(nullptr);
		//** Render scene end **//

		//** Render UI begin **//
		_renderer->setViewportSize(_width, _height);
		_renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

		_renderer->beginUI();
		ImGuizmo::BeginFrame();
		_windowManager->update(_width, _height);
		_renderer->endUI();
		//** Render UI end **//

		_timeAccumulator += _time->getDeltaTime();

		if (_timeAccumulator >= 0.5f)
		{
			_timeAccumulator = 0.0f;
			setTitle(("Core Engine: " + std::to_string(_time->getFramesPerSecond()) + " FPS | " + std::to_string(_time->getFrameTimeMs()) + " ms")
						 .c_str());
		}
	}

	/* EDITOR */

	void EditorApp::init()
	{
		_wnd = new MainWindow(this);

		float iconFontSize = 15.0f;

		ImGuiIO& io = ImGui::GetIO();
		_mainFont = new Font(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), iconFontSize);

		static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_16_FK, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = iconFontSize;
		io.Fonts->AddFontFromFileTTF(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts", FONT_ICON_FILE_NAME_FK).std_str().c_str(),
			iconFontSize, &icons_config, icons_ranges);

		Font::rebuildFonts();

		_mainFont->setDefault();
	}

	void EditorApp::destroy()
	{
		delete _mainFont;

		_mainFont = nullptr;
		_wnd = nullptr;
	}
} // namespace Editor