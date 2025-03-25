#include "ObjectPicker.h"

#include <Components/Camera.h>
#include <System/InputManager.h>

namespace Editor
{
	Core::Scene* ObjectPicker::_scene = nullptr;
	Core::Camera* ObjectPicker::_camera = nullptr;

	void ObjectPicker::init(Core::Scene* scene, Core::Camera* camera)
	{
		_scene = scene;
		_camera = camera;

		Core::InputManager::singleton()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
			pickObject(static_cast<int>(mb), x, y);
		});
	}

	void ObjectPicker::pickObject(int btn, int x, int y)
	{
		//TODO: Raycast
	}
}