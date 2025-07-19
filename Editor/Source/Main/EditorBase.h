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
}

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

	Editor::MainMenu* mainMenu = nullptr;
	Editor::SceneWindow* sceneWindow = nullptr;
	Editor::ObjectWindow* objectWindow = nullptr;
	Editor::CSGObjectWindow* csgObjectWindow = nullptr;
	Editor::InspectorWindow* inspectorWindow = nullptr;
	Editor::HierarchyWindow* hierarchyWindow = nullptr;
	Editor::AssetsWindow* assetsWindow = nullptr;
	Editor::GizmoWindow* gizmoWindow = nullptr;
	Editor::CSGEditWindow* csgEditWindow = nullptr;

	Editor::CSGModifier* csgModifier = nullptr;

	int init();
	void loop();
	void destroy();

public:
	int run();
};