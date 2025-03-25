#include "ObjectPicker.h"

#include <Components/Camera.h>
#include <System/InputManager.h>

namespace Editor
{
	Core::Camera* ObjectPicker::_camera = nullptr;

	void ObjectPicker::init(Core::Camera* camera)
	{
		_camera = camera;

		Core::InputManager::singleton()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
			pickObject(static_cast<int>(mb), x, y);
		});
	}

	void ObjectPicker::pickObject(int btn, int x, int y)
	{

	}
}