#pragma once

namespace Core
{
	class Camera;
}

namespace Editor
{
	class ObjectPicker
	{
	private:
		static Core::Camera* _camera;

		static void pickObject(int btn, int x, int y);

	public:
		static void init(Core::Camera* camera);
	};
}