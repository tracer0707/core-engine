#pragma once

namespace Core
{
	class Scene;
	class Camera;
}

namespace Editor
{
	class ObjectPicker
	{
	private:
		static Core::Scene* _scene;
		static Core::Camera* _camera;

		static bool _isMouseInView;
		static float _offsetX;
		static float _offsetY;

		static void pickObject(int x, int y);

	public:
		static void init(Core::Scene* scene, Core::Camera* camera);
		static void update(bool isMouseInView, float offsetX, float offsetY);
	};
}