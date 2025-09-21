#include "Object.h"

#include "../Components/Component.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"

namespace Core
{
	Object::Object(Renderer* renderer)
	{
		_renderer = renderer;
		addComponent<Core::Transform*>();
	}

	Object::~Object()
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			component->owner = nullptr;
			delete component;
		}

		_components.clear();
		_renderer = nullptr;
	}

	void Object::update(float& dt)
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			component->update(dt);
		}
	}

	void Object::render(Camera* camera)
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			component->render(camera);
		}
	}

	Component* Object::findComponent(unsigned int type)
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			if (component->getComponentType() == type)
				return component;
		}

		return nullptr;
	}

	Component* Object::addComponent(unsigned int type)
	{
		Component* newComponent = nullptr;

		if (type == COMPONENT_CAMERA)
		{
			newComponent = new Camera(this, _renderer);
		}
		else if (type == COMPONENT_MESHRENDERER)
		{
			newComponent = new MeshRenderer(this, _renderer);
		}
		else if (type == COMPONENT_TRANSFORM)
		{
			newComponent = new Transform(this);
		}

		assert(newComponent != nullptr && "Unknown component");

		_components.add(newComponent);

		return newComponent;
	}

	void Object::removeComponent(Component* component)
	{
		int idx = -1;

		for (int i = 0; i < _components.count(); ++i)
		{
			Component* comp = _components.get(i);
			if (comp == component)
			{
				idx = i;
				delete comp;
				break;
			}
		}

		if (idx > -1)
		{
			_components.removeAt(idx);
		}
	}
}