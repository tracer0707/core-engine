#pragma once

namespace Core
{
	class Renderer;

	class Camera
	{
	public:
		Camera() = default;

		const void frame(Renderer* renderer);
	};
}