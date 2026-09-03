#include "CameraController.h"

#include <../sdl/include/SDL/SDL.h>
#include <glm/gtc/type_ptr.hpp>

#include <Core/System/EventHandler.h>
#include <Core/System/InputManager.h>
#include <Core/System/Time.h>
#include <Core/Interface/Transform.h>
#include <Core/Scene/Object.h>
#include <Core/Math/Mathf.h>

#include "EditorCamera.h"

namespace Editor
{
	CameraController::CameraController(Core::InputManager* inputManager, Core::Time* time, EditorCamera* camera)
	{
		_time = time;
		_camera = camera;
		_inputManager = inputManager;

		_inputManager->subscribeMouseDownEvent([this](Core::InputManager::MouseButton mb, int x, int y) { mouseDown(x, y, static_cast<int>(mb)); });

		_inputManager->subscribeMouseUpEvent([this](Core::InputManager::MouseButton mb, int x, int y) { mouseUp(x, y, static_cast<int>(mb)); });

		_inputManager->subscribeMouseMoveEvent([this](int x, int y) { mouseMove(x, y); });

		_inputManager->subscribeMouseWheelEvent([this](int x, int y) { mouseWheel(x, y); });
	}

	void CameraController::update(bool isMouseInView)
	{
		if (!enabled) return;

		hovered = isMouseInView;

		ctrlPressed = _inputManager->getKey(SDL_SCANCODE_LCTRL);
		shiftPressed = _inputManager->getKey(SDL_SCANCODE_LSHIFT);

		if (shiftPressed)
			cameraSpeed = cameraSpeedFast;
		else
			cameraSpeed = cameraSpeedNormal;

		if (rButtonDown)
		{
			float dt = _time->getDeltaTime();

			Core::Transform* t = _camera->getTransform();

			if (_inputManager->getKey(SDL_SCANCODE_W))
			{
				t->translate(t->getForward() * cameraSpeed * dt, true);
			}

			if (_inputManager->getKey(SDL_SCANCODE_S))
			{
				t->translate(-t->getForward() * cameraSpeed * dt, true);
			}

			if (_inputManager->getKey(SDL_SCANCODE_A))
			{
				t->translate(-t->getRight() * cameraSpeed * dt, true);
			}

			if (_inputManager->getKey(SDL_SCANCODE_D))
			{
				t->translate(t->getRight() * cameraSpeed * dt, true);
			}

			if (_inputManager->getKey(SDL_SCANCODE_Q))
			{
				t->translate(t->getUp() * cameraSpeed * dt, true);
			}

			if (_inputManager->getKey(SDL_SCANCODE_E))
			{
				t->translate(-t->getUp() * cameraSpeed * dt, true);
			}
		}
	}

	void CameraController::mouseDown(int x, int y, int mb)
	{
		if (!enabled || !hovered) return;

		Core::InputManager::MouseButton mbe = static_cast<Core::InputManager::MouseButton>(mb);

		if (mbe == Core::InputManager::MouseButton::MBE_LEFT)
		{
			if (!rButtonDown && !mButtonDown)
			{
				lButtonDown = true;
			}
		}

		if (mbe == Core::InputManager::MouseButton::MBE_RIGHT)
		{
			if (!lButtonDown && !mButtonDown) rButtonDown = true;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_MIDDLE)
		{
			if (!lButtonDown && !rButtonDown) mButtonDown = true;
		}
	}

	void CameraController::mouseUp(int x, int y, int mb)
	{
		if (!enabled) return;

		Core::InputManager::MouseButton mbe = static_cast<Core::InputManager::MouseButton>(mb);

		if (mbe == Core::InputManager::MouseButton::MBE_LEFT)
		{
			lButtonDown = false;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_RIGHT)
		{
			rButtonDown = false;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_MIDDLE)
		{
			mButtonDown = false;
		}
	}

	void CameraController::mouseMove(int x, int y)
	{
		if (!enabled) return;

		float rOffsetX = x - prevMousePos.x;
		float rOffsetY = y - prevMousePos.y;

		Core::Transform* t = _camera->getTransform();

		if (rButtonDown)
		{
			rOffsetX *= 0.15f;
			rOffsetY *= 0.15f;

			t->yaw(-rOffsetX, true);
			t->pitch(-rOffsetY, false);
		}

		if (mButtonDown)
		{
			rOffsetX *= cameraSpeedNormal * 0.004f;
			rOffsetY *= cameraSpeedNormal * 0.004f;

			glm::vec3 vCamPos = t->getPosition();
			glm::vec3 vDirUp = -t->getUp();
			glm::vec3 vDirRight = -t->getRight();

			vCamPos += (vDirRight * rOffsetX) + (vDirUp * rOffsetY);
			t->setPosition(vCamPos);
		}

		prevMousePos = glm::vec2(x, y);
	}

	void CameraController::mouseWheel(int x, int y)
	{
		if (!enabled || !hovered) return;

		if (!lButtonDown)
		{
			Core::Transform* t = _camera->getTransform();

			glm::vec3 vCamPos = t->getPosition();
			glm::vec3 vCamDir = t->getForward();
			vCamPos += vCamDir * (float)y * cameraSpeedNormal * 0.1f;
			t->setPosition(vCamPos);
		}
	}
} // namespace Editor