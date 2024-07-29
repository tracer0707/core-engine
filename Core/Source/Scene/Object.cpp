#include "Object.h"

#include "../Components/Component.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"

namespace Core
{
	Object::Object()
	{
	}

	Object::~Object()
	{
		for (int i = 0; i < components.count(); ++i)
		{
			Component* component = components.get(i);
			delete component;
		}

		components.clear();
	}

	void Object::update(float& dt)
	{
		for (int i = 0; i < components.count(); ++i)
		{
			Component* component = components.get(i);
			component->update(dt);
		}
	}

	void Object::render()
	{
		for (int i = 0; i < components.count(); ++i)
		{
			Component* component = components.get(i);
			component->render();
		}
	}

	Component* Object::findComponent(UInt32 type)
	{
		for (int i = 0; i < components.count(); ++i)
		{
			Component* component = components.get(i);
			if (component->getComponentType() == type)
				return component;
		}

		return nullptr;
	}

	Component* Object::addComponent(UInt32 type)
	{
		Component* newComponent = nullptr;

		if (type == COMPONENT_CAMERA)
			newComponent = new Camera(this);

		else if (type == COMPONENT_MESHRENDERER)
			newComponent = new MeshRenderer(this);

		else if (type == COMPONENT_TRANSFORM)
			newComponent = new Transform(this);

		if (newComponent != nullptr)
			components.add(newComponent);

		return newComponent;
	}

	void Object::removeComponent(Component* component)
	{
		int idx = -1;

		for (int i = 0; i < components.count(); ++i)
		{
			Component* comp = components.get(i);
			if (comp == component)
			{
				idx = i;
				delete comp;
				break;
			}
		}

		if (idx > -1)
			components.removeAt(idx);
	}
}