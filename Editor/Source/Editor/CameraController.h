#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <Core/Shared/List.h>

namespace Core
{
	class Camera;
	class Time;
	class InputManager;
}

namespace Editor
{
	class CameraController
	{
	private:
		static Core::Camera* _camera;
		static Core::Time* _time;
		static Core::InputManager* _inputManager;

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
		static void init(Core::InputManager* inputManager, Core::Time* time, Core::Camera* camera);
		static void update(bool isMouseInView);
	};
}