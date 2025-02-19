#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <Shared/List.h>

namespace Core
{
	class Camera;
}

namespace Editor
{
	class CameraController
	{
	private:
		static Core::Camera* _camera;

		static bool hovered;

		static bool lButtonDown;
		static bool rButtonDown;
		static bool mButtonDown;
		static bool ctrlPressed;
		static bool shiftPressed;
		static bool mouseOver;
		static glm::vec2 prevMousePos;

		static float cameraSpeed;
		static float cameraSpeedNormal;
		static float cameraSpeedFast;

		static void mouseDown(int x, int y, int mb);
		static void mouseUp(int x, int y, int mb);
		static void mouseMove(int x, int y);
		static void mouseWheel(int x, int y);

	public:
		static void init(Core::Camera* camera);
		static void update(bool isMouseInView);
	};
}