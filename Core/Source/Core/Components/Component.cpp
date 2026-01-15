#include "Component.h"

namespace Core
{
	Component::Component(Object* owner)
	{
		_owner = owner;
	}

	Component::~Component()
	{
		_owner = nullptr;
	}

	void Component::update(float& dt) {}

	void Component::render(Camera* camera) {}
} // namespace Core