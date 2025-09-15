#pragma once

#include <System/Application.h>
#include <System/Window.h>

namespace Core
{
	class Object;
	class Camera;
	class Transform;
	class Scene;
	class RenderTexture;
}

namespace Editor
{
	class WindowManager;
	class MainMenu;
	class SceneWindow;
	class ObjectWindow;
	class CSGObjectWindow;
	class InspectorWindow;
	class HierarchyWindow;
	class AssetsWindow;
	class GizmoWindow;
	class CSGEditWindow;
	class CSGModifier;
	class Font;

	class Editor : public Core::Application
	{
	private:
		class MainWindow : public Core::Window
		{
			friend class Editor;

		private:
			MainWindow(Core::Application* app);
			virtual ~MainWindow();

			virtual void update();
			virtual void render();

			Core::Object* _cameraObject = nullptr;
			Core::Camera* _camera = nullptr;
			Core::Scene* _scene = nullptr;
			Core::RenderTexture* _renderTexture = nullptr;

			WindowManager* _windowManager = nullptr;

			MainMenu* _mainMenu = nullptr;
			SceneWindow* _sceneWindow = nullptr;
			ObjectWindow* _objectWindow = nullptr;
			CSGObjectWindow* _csgObjectWindow = nullptr;
			InspectorWindow* _inspectorWindow = nullptr;
			HierarchyWindow* _hierarchyWindow = nullptr;
			AssetsWindow* _assetsWindow = nullptr;
			GizmoWindow* _gizmoWindow = nullptr;
			CSGEditWindow* _csgEditWindow = nullptr;

			CSGModifier* _csgModifier = nullptr;
		};
	
		Font* _mainFont = nullptr;
		MainWindow* _wnd = nullptr;

		virtual void init();
		virtual void destroy();
	};
}