#include "Object.h"

#include "../Components/Component.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Interface/Transform.h"

namespace Core
{
	Object::Object(Renderer* renderer)
	{
		_renderer = renderer;
		_transform = new Transform(this);
	}

	Object::~Object()
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			delete component;
		}

		_components.clear();

		delete _transform;

		_transform = nullptr;
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

	void Object::render(glm::mat4& view, glm::mat4& proj)
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			component->render(view, proj);
		}
	}

	Component* Object::findComponent(ComponentType type) const
	{
		for (int i = 0; i < _components.count(); ++i)
		{
			Component* component = _components.get(i);
			if (component->getComponentType() == type) return component;
		}

		return nullptr;
	}

	Component* Object::addComponent(ComponentType type)
	{
		Component* newComponent = nullptr;

		if (type == ComponentType::Camera)
		{
			newComponent = new Camera(this, _renderer);
		}
		else if (type == ComponentType::MeshRenderer)
		{
			newComponent = new MeshRenderer(this, _renderer);
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
} // namespace Core