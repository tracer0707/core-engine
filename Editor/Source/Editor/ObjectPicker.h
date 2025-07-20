#pragma once

namespace Core
{
	class Scene;
	class Camera;
}

namespace Editor
{
	class WindowManager;

	class ObjectPicker
	{
	private:
		static WindowManager* _windowManager;

		static Core::Scene* _scene;
		static Core::Camera* _camera;

		static bool _isGizmoWasUsed;
		static bool _isMouseInView;
		static bool _isMouseWasMoved;

		static bool _isMouseLPressed;
		static bool _isMouseRPressed;
		static bool _isMouseMPressed;

		static float _offsetX;
		static float _offsetY;

		static void pickObject(int x, int y);

	public:
		static void init(WindowManager* windowManager, Core::Scene* scene, Core::Camera* camera);
		static void update(bool& isMouseInView, bool& isGizmoWasUsed, float& offsetX, float& offsetY);
	};
}