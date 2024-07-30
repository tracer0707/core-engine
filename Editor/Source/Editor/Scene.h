#pragma once

namespace Core
{
	class Camera;
}

namespace Editor
{
	class Scene
	{
	public:
		static void renderGrid(Core::Camera* camera);
	};
}