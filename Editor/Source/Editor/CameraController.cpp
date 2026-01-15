#include "CameraController.h"

#include <../sdl/include/SDL/SDL.h>
#include <glm/gtc/type_ptr.hpp>

#include <Core/System/EventHandler.h>
#include <Core/System/InputManager.h>
#include <Core/System/Time.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/Scene/Object.h>
#include <Core/Math/Mathf.h>

namespace Editor
{
	Core::Camera* CameraController::_camera = nullptr;
	Core::Time* CameraController::_time = nullptr;
	Core::InputManager* CameraController::_inputManager = nullptr;

	bool CameraController::hovered = false;

	bool CameraController::lButtonDown = false;
	bool CameraController::rButtonDown = false;
	bool CameraController::mButtonDown = false;
	bool CameraController::ctrlPressed = false;
	bool CameraController::shiftPressed = false;
	bool CameraController::mouseOver = false;

	float CameraController::cameraSpeed = 5.0f;
	float CameraController::cameraSpeedNormal = 5.0f;
	float CameraController::cameraSpeedFast = 10.0f;

	glm::vec2 CameraController::prevMousePos = glm::vec2(0, 0);

	void CameraController::init(Core::InputManager* inputManager, Core::Time* time, Core::Camera* camera)
	{
		_time = time;
		_camera = camera;
		_inputManager = inputManager;

		_inputManager->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y) { mouseDown(x, y, static_cast<int>(mb)); });

		_inputManager->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) { mouseUp(x, y, static_cast<int>(mb)); });

		_inputManager->subscribeMouseMoveEvent([=](int x, int y) { mouseMove(x, y); });

		_inputManager->subscribeMouseWheelEvent([=](int x, int y) { mouseWheel(x, y); });
	}

	void CameraController::update(bool isMouseInView)
	{
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

			Core::Transform* t = _camera->getOwner()->findComponent<Core::Transform*>();

			if (_inputManager->getKey(SDL_SCANCODE_W))
			{
				t->translate(-t->getForward() * cameraSpeed * dt);
			}

			if (_inputManager->getKey(SDL_SCANCODE_S))
			{
				t->translate(t->getForward() * cameraSpeed * dt);
			}

			if (_inputManager->getKey(SDL_SCANCODE_A))
			{
				t->translate(-t->getRight() * cameraSpeed * dt);
			}

			if (_inputManager->getKey(SDL_SCANCODE_D))
			{
				t->translate(t->getRight() * cameraSpeed * dt);
			}

			if (_inputManager->getKey(SDL_SCANCODE_Q))
			{
				t->translate(-t->getUp() * cameraSpeed * dt);
			}

			if (_inputManager->getKey(SDL_SCANCODE_E))
			{
				t->translate(t->getUp() * cameraSpeed * dt);
			}
		}
	}

	void CameraController::mouseDown(int x, int y, int mb)
	{
		if (!hovered) return;

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
		float rOffsetX = x - prevMousePos.x;
		float rOffsetY = y - prevMousePos.y;

		Core::Transform* t = _camera->getOwner()->findComponent<Core::Transform*>();

		if (rButtonDown)
		{
			rOffsetX *= 0.15f;
			rOffsetY *= 0.15f;

			t->yaw(-rOffsetX, true);
			t->pitch(-rOffsetY, false);
		}

		if (mButtonDown)
		{
			rOffsetX *= cameraSpeedNormal * 0.005f;
			rOffsetY *= cameraSpeedNormal * 0.005f;

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
		if (!hovered) return;

		if (!lButtonDown)
		{
			Core::Transform* t = _camera->getOwner()->findComponent<Core::Transform*>();

			glm::vec3 vCamPos = t->getPosition();
			glm::vec3 vCamDir = t->getForward();
			vCamPos += vCamDir * (float)y * cameraSpeedNormal * 0.1f;
			t->setPosition(vCamPos);
		}
	}
} // namespace Editor