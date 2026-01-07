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
#include "../Editor/Windows/CSGObjectWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/Windows/ContentWindow.h"
#include "../Editor/Windows/GizmoWindow.h"
#include "../Editor/Windows/CSGEditWindow.h"

#include "../Editor/Modifiers/ModifierManager.h"
#include "../Editor/Modifiers/CSGModifier.h"

#include "../Editor/CameraController.h"
#include "../Editor/Rendering.h"

#include "../System/ThumbCacheManager.h"
#include "../Shared/IconsForkAwesome.h"

namespace Editor
{
	/* WINDOW */

	EditorApp::MainWindow::MainWindow(EditorApp* app) : Window(app, "Project Manager", 1366, 768)
	{
		_thumbCacheManager = new ThumbCacheManager(app);

		_scene = new Core::Scene(_renderer);

		_cameraObject = _scene->createObject();
		_camera = _cameraObject->addComponent<Core::Camera*>();
		Core::Transform* cameraTransform = (Core::Transform*)_cameraObject->findComponent<Core::Transform*>();

		_renderTexture = _contentManager->createRenderTexture(512, 512);
		_camera->setRenderTexture(_renderTexture);

		_scene->setMainCamera(_camera);

		cameraTransform->setPosition(glm::vec3(0.0f, 5.0f, 5.0f));
		cameraTransform->setRotation(glm::vec3(-10.0f, 0, 0));

		_gridBuffer = _renderer->createBuffer(2048, 0);

		_windowManager = new WindowManager(app);
		_windowManager->setTime(_time);
		_windowManager->setRenderer(_renderer);
		_windowManager->setContentManager(_contentManager);
		_windowManager->setInputManager(_inputManager);
		_windowManager->setEventHandler(_eventHandler);
		_windowManager->setThumbCacheManager(_thumbCacheManager);

		_csgModifier = ModifierManager::singleton()->addModifier<CSGModifier*>();

		_mainMenu = new MainMenu();
		_windowManager->setMenuBar(_mainMenu->getMenuBar());

		_sceneWindow = _windowManager->addWindow<SceneWindow*>();
		_sceneWindow->setTime(_time);
		_sceneWindow->setScene(_scene);
		_sceneWindow->setRenderTexture(_renderTexture);

		_inspectorWindow = _windowManager->addWindow<InspectorWindow*>();
		_hierarchyWindow = _windowManager->addWindow<HierarchyWindow*>();
		_contentWindow = _windowManager->addWindow<ContentWindow*>();

		_contentWindow->setContentDir(app->getContentPath());

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
			auto dockContent = _contentWindow->dock(DockDirection::Down, dockHierarchy.area2, 0.3f);
			auto dockScene = _sceneWindow->dock(DockDirection::None, dockContent.area2, 0.7f);
		});

		_windowManager->initWindows();
	}

	EditorApp::MainWindow::~MainWindow()
	{
		ModifierManager::singleton()->destroy();

		_renderer->deleteBuffer(_gridBuffer);

		delete _windowManager;
		delete _scene;

		_gridBuffer = nullptr;
		_windowManager = nullptr;
		_scene = nullptr;
	}

	void EditorApp::MainWindow::update()
	{
		ModifierManager::singleton()->update();
	}

	void EditorApp::MainWindow::render()
	{
		//** Render scene begin **//
		_camera->getRenderTexture()->bind();

		int viewportWidth = _renderTexture->getWidth();
		int viewportHeight = _renderTexture->getHeight();

		_renderer->setViewportSize(viewportWidth, viewportHeight);
		_renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.4f, 0.4f, 0.4f, 1.0f));

		Rendering::renderGrid(_renderer, _gridBuffer, _contentManager->getDefaultMaterial(), _camera);
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