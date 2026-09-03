#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>
#include <Core/System/Application.h>
#include <Core/System/Window.h>

namespace Core
{
	class Scene;
	class FrameBuffer;
	class VertexBuffer;
} // namespace Core

namespace Editor
{
	class WindowManager;
	class MainMenu;
	class SceneWindow;
	class ObjectWindow;
	class InspectorWindow;
	class HierarchyWindow;
	class ContentWindow;
	class ContentImportWindow;
	class ToolWindow;
	class Font;
	class GizmoRenderer;
	class EditorCamera;

	class EditorApp : public Core::Application
	{
		public:
			class MainWindow : public Core::Window
			{
					friend class EditorApp;

				private:
					MainWindow(EditorApp* app);
					virtual ~MainWindow();

					virtual void update();

					float _timeAccumulator = 0.0f;

					EditorCamera* _camera = nullptr;
					Core::Scene* _scene = nullptr;
					Core::VertexBuffer* _gridBuffer = nullptr;
					const Core::FrameBuffer* _frameBuffer = nullptr;

					GizmoRenderer* _gizmoRenderer = nullptr;
					WindowManager* _windowManager = nullptr;

					MainMenu* _mainMenu = nullptr;
					SceneWindow* _sceneWindow = nullptr;
					ObjectWindow* _objectWindow = nullptr;
					InspectorWindow* _inspectorWindow = nullptr;
					HierarchyWindow* _hierarchyWindow = nullptr;
					ContentWindow* _contentWindow = nullptr;
					ContentImportWindow* _contentImportWindow = nullptr;
					ToolWindow* _toolWindow = nullptr;

				public:
					void setScene(Core::Scene* value);
					Core::Scene* getScene() const { return _scene; }
			};

		private:
			Font* _mainFont = nullptr;
			MainWindow* _wnd = nullptr;

			virtual void init();
			virtual void destroy();
	};
} // namespace Editor