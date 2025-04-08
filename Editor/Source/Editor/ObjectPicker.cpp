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

	bool ObjectPicker::_isMouseInView = false;
	float ObjectPicker::_offsetX = 0;
	float ObjectPicker::_offsetY = 0;

	void ObjectPicker::init(Core::Scene* scene, Core::Camera* camera)
	{
		_scene = scene;
		_camera = camera;

		Core::InputManager::singleton()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				if (!_isMouseInView) return;
				pickObject(x, y);
			}
		});
	}

	void ObjectPicker::update(bool isMouseInView, float offsetX, float offsetY)
	{
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
			glm::mat4 mtx = transform->getLocalTransformMatrix();

			Gizmo::singleton()->setModelMatrix(&mtx);
		}
	}
}