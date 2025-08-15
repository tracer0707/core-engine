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

	class Editor : public Core::Application
	{
	private:
		class MainWindow : public Core::Window
		{
			friend class Editor;

		private:
			MainWindow();
			virtual ~MainWindow();

			virtual void update();
			virtual void render();

			Core::Object* cameraObject = nullptr;
			Core::Camera* camera = nullptr;
			Core::Scene* scene = nullptr;
			Core::RenderTexture* renderTexture = nullptr;

			WindowManager* windowManager = nullptr;

			MainMenu* mainMenu = nullptr;
			SceneWindow* sceneWindow = nullptr;
			ObjectWindow* objectWindow = nullptr;
			CSGObjectWindow* csgObjectWindow = nullptr;
			InspectorWindow* inspectorWindow = nullptr;
			HierarchyWindow* hierarchyWindow = nullptr;
			AssetsWindow* assetsWindow = nullptr;
			GizmoWindow* gizmoWindow = nullptr;
			CSGEditWindow* csgEditWindow = nullptr;

			CSGModifier* csgModifier = nullptr;
		};
	
		MainWindow* wnd = nullptr;

		virtual void init();
		virtual void destroy();
	};
}