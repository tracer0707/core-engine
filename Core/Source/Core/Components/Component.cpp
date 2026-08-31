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
} // namespace Core