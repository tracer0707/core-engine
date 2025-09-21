#include "Transform.h"

#include "ComponentList.h"

namespace Core
{
	Transform::Transform(Object* owner) : Transformable(), Component(owner)
	{
		
	}

	Transform::~Transform() {}

	unsigned int Transform::getComponentType()
	{
		return COMPONENT_TRANSFORM;
	}
}