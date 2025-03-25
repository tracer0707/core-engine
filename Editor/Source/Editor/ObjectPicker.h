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

		static void pickObject(int btn, int x, int y);

	public:
		static void init(Core::Scene* scene, Core::Camera* camera);
	};
}