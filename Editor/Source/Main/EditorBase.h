#pragma once

namespace Core
{
	class DeviceContext;
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

	class EditorBase
	{
	private:
		bool isRunning = false;

		Core::DeviceContext* ctx = nullptr;

		Core::Object* object = nullptr;
		Core::Camera* camera = nullptr;
		Core::Transform* transform = nullptr;
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

		int init();
		void loop();
		void destroy();

	public:
		int run();
	};
}