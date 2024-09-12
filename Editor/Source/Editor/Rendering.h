#pragma once

namespace Core
{
	class Camera;
}

namespace Editor
{
	class Rendering
	{
	public:
		static void renderGrid(Core::Camera* camera);
	};
}