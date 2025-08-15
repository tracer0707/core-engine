#pragma once

namespace Core
{
	class Camera;
	class Renderer;
	class Material;
}

namespace Editor
{
	class Rendering
	{
	public:
		static void renderGrid(Core::Renderer* renderer, Core::Material* material, Core::Camera* camera);
	};
}