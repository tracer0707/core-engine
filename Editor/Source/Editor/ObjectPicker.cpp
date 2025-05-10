#include "ObjectPicker.h"

#include <imgui.h>
#include <Scene/Object.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <System/InputManager.h>

#include "Gizmo.h"
#include "Windows/WindowManager.h"
#include "../SceneUtils/Raycast.h"

namespace Editor
{
	Core::Scene* ObjectPicker::_scene = nullptr;
	Core::Camera* ObjectPicker::_camera = nullptr;

	bool ObjectPicker::_isGizmoWasUsed = false;
	bool ObjectPicker::_isMouseInView = false;
	bool ObjectPicker::_isMouseWasMoved = false;
	bool ObjectPicker::_isMouseLPressed = false;
	bool ObjectPicker::_isMouseRPressed = false;
	bool ObjectPicker::_isMouseMPressed = false;

	float ObjectPicker::_offsetX = 0;
	float ObjectPicker::_offsetY = 0;

	void ObjectPicker::init(Core::Scene* scene, Core::Camera* camera)
	{
		_scene = scene;
		_camera = camera;

		Core::InputManager::singleton()->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y)
		{
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				_isMouseWasMoved = false;
				_isMouseLPressed = true;
			}

			if (mb == Core::InputManager::MouseButton::MBE_RIGHT)
			{
				_isMouseRPressed = true;
			}

			if (mb == Core::InputManager::MouseButton::MBE_MIDDLE)
			{
				_isMouseMPressed = true;
			}
		});

		Core::InputManager::singleton()->subscribeMouseMoveEvent([=](int x, int y)
		{
			if (_isMouseLPressed)
			{
				_isMouseWasMoved = true;
			}
		});

		Core::InputManager::singleton()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y)
		{
			if (mb == Core::InputManager::MouseButton::MBE_LEFT
				&& !_isMouseRPressed
				&& !_isMouseMPressed
				&& !_isMouseWasMoved
				&& !_isGizmoWasUsed
				&& _isMouseInView)
			{
				pickObject(x, y);

				_isMouseLPressed = false;
				_isMouseWasMoved = false;
			}

			if (mb == Core::InputManager::MouseButton::MBE_RIGHT)
			{
				_isMouseRPressed = false;
			}

			if (mb == Core::InputManager::MouseButton::MBE_MIDDLE)
			{
				_isMouseMPressed = false;
			}
		});
	}

	void ObjectPicker::update(bool& isMouseInView, bool& isGizmoWasUsed, float& offsetX, float& offsetY)
	{
		_isGizmoWasUsed = isGizmoWasUsed;
		_isMouseInView = isMouseInView;
		_offsetX = offsetX;
		_offsetY = offsetY;
	}

	void ObjectPicker::pickObject(int x, int y)
	{
		Core::Ray ray = _camera->getCameraToViewportRay(x, y, _offsetX, _offsetY);

		RaycastHit hit;
		Raycast::hitTest(_scene, ray, &hit);

		if (hit.object != nullptr)
		{
			Core::Transform* transform = (Core::Transform*)hit.object->findComponent<Core::Transform*>();
			glm::mat4& mtx = transform->getTransformMatrix();

			Gizmo::singleton()->setModelMatrix(&mtx);
		}
		else
		{
			Gizmo::singleton()->setModelMatrix(nullptr);
		}
	}
}