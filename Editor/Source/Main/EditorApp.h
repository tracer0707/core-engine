#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>
#include <Core/System/Application.h>
#include <Core/System/Window.h>

namespace Core
{
	class Object;
	class Camera;
	class Transform;
	class Scene;
	class RenderTexture;
	class VertexBuffer;
} // namespace Core

namespace Editor
{
	class WindowManager;
	class MainMenu;
	class SceneWindow;
	class ObjectWindow;
	class CSGObjectWindow;
	class InspectorWindow;
	class HierarchyWindow;
	class ContentWindow;
	class ContentImportWindow;
	class GizmoWindow;
	class CSGEditWindow;
	class CSGModifier;
	class Font;

	class EditorApp : public Core::Application
	{
		private:
			class MainWindow : public Core::Window
			{
					friend class EditorApp;

				private:
					MainWindow(EditorApp* app);
					virtual ~MainWindow();

					virtual void update();
					virtual void render();

					Core::Object* _cameraObject = nullptr;
					Core::Camera* _camera = nullptr;
					Core::Scene* _scene = nullptr;
					Core::RenderTexture* _renderTexture = nullptr;
					Core::VertexBuffer* _gridBuffer = nullptr;

					WindowManager* _windowManager = nullptr;

					MainMenu* _mainMenu = nullptr;
					SceneWindow* _sceneWindow = nullptr;
					ObjectWindow* _objectWindow = nullptr;
					CSGObjectWindow* _csgObjectWindow = nullptr;
					InspectorWindow* _inspectorWindow = nullptr;
					HierarchyWindow* _hierarchyWindow = nullptr;
					ContentWindow* _contentWindow = nullptr;
					ContentImportWindow* _contentImportWindow = nullptr;
					GizmoWindow* _gizmoWindow = nullptr;
					CSGEditWindow* _csgEditWindow = nullptr;

					CSGModifier* _csgModifier = nullptr;
			};

			Font* _mainFont = nullptr;
			MainWindow* _wnd = nullptr;

			virtual void init();
			virtual void destroy();

		public:
			
	};
} // namespace Editor