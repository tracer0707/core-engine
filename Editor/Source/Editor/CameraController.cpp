#include "CameraController.h"

#include <../sdl/include/SDL/SDL.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include <System/EventHandler.h>
#include <System/InputManager.h>
#include <System/Time.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Scene/Object.h>
#include <Math/Mathf.h>

namespace Editor
{
	Core::Camera* CameraController::_camera = nullptr;

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

	void CameraController::init(Core::Camera* camera)
	{
		_camera = camera;

		Core::InputManager::singleton()->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y)
			{
				mouseDown(x, y, static_cast<int>(mb));
			}
		);

		Core::InputManager::singleton()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y)
			{
				mouseUp(x, y, static_cast<int>(mb));
			}
		);

		Core::InputManager::singleton()->subscribeMouseMoveEvent([=](int x, int y)
			{
				mouseMove(x, y);
			}
		);

		Core::InputManager::singleton()->subscribeMouseWheelEvent([=](int x, int y)
			{
				mouseWheel(x, y);
			}
		);
	}

	void CameraController::update(bool isMouseInView)
	{
		hovered = isMouseInView;

		ctrlPressed = Core::InputManager::singleton()->getKey(SDL_SCANCODE_LCTRL);
		shiftPressed = Core::InputManager::singleton()->getKey(SDL_SCANCODE_LSHIFT);

		if (shiftPressed)
			cameraSpeed = cameraSpeedFast;
		else
			cameraSpeed = cameraSpeedNormal;

		if (rButtonDown)
		{
			float dt = Core::Time::getDeltaTime();

			Core::Transform* t = _camera->getOwner()->findComponent<Core::Transform*>();

			if (Core::InputManager::singleton()->getKey(SDL_SCANCODE_W))
			{
				t->translate(glm::vec3(0, 0, 1) * cameraSpeed * dt);
			}

			if (Core::InputManager::singleton()->getKey(SDL_SCANCODE_S))
			{
				t->translate(glm::vec3(0, 0, -1) * cameraSpeed * dt);
			}

			if (Core::InputManager::singleton()->getKey(SDL_SCANCODE_A))
			{
				t->translate(glm::vec3(-1, 0, 0) * cameraSpeed * dt);
			}

			if (Core::InputManager::singleton()->getKey(SDL_SCANCODE_D))
			{
				t->translate(glm::vec3(1, 0, 0) * cameraSpeed * dt);
			}

			if (Core::InputManager::singleton()->getKey(SDL_SCANCODE_Q))
			{
				t->translate(glm::vec3(0, 1, 0) * cameraSpeed * dt);
			}

			if (Core::InputManager::singleton()->getKey(SDL_SCANCODE_E))
			{
				t->translate(glm::vec3(0, -1, 0) * cameraSpeed * dt);
			}
		}
	}

	void CameraController::mouseDown(int x, int y, int mb)
	{
		if (!hovered)
			return;

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
			if (!lButtonDown && !mButtonDown)
				rButtonDown = true;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_MIDDLE)
		{
			if (!lButtonDown && !rButtonDown)
				mButtonDown = true;
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
		if (!hovered)
			return;

		if (!lButtonDown)
		{
			Core::Transform* t = _camera->getOwner()->findComponent<Core::Transform*>();

			glm::vec3 vCamPos = t->getPosition();
			glm::vec3 vCamDir = t->getForward();
			vCamPos += vCamDir * (float)y * cameraSpeedNormal * 0.1f;
			t->setPosition(vCamPos);
		}
	}
}