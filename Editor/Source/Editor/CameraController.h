#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <Core/Shared/List.h>

namespace Core
{
	class Time;
	class InputManager;
}

namespace Editor
{
	class EditorCamera;

	class CameraController
	{
	private:
		EditorCamera* _camera = nullptr;
		Core::Time* _time = nullptr;
		Core::InputManager* _inputManager = nullptr;

		bool enabled = false;
		bool hovered = false;

		bool lButtonDown = false;
		bool rButtonDown = false;
		bool mButtonDown = false;
		bool ctrlPressed = false;
		bool shiftPressed = false;
		bool mouseOver = false;
		glm::vec2 prevMousePos = glm::vec2(0, 0);

		float cameraSpeed = 5.0f;
		float cameraSpeedNormal = 5.0f;
		float cameraSpeedFast = 10.0f;

		void mouseDown(int x, int y, int mb);
		void mouseUp(int x, int y, int mb);
		void mouseMove(int x, int y);
		void mouseWheel(int x, int y);

	public:
		CameraController(Core::InputManager* inputManager, Core::Time* time, EditorCamera* camera);

		void update(bool isMouseInView);
		void setEnabled(bool value) { enabled = value; }
	};
}