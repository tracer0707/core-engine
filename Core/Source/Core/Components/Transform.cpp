#include "Transform.h"

#include "ComponentList.h"

namespace Core
{
    Transform::Transform(Object* owner) : Component(owner), Transformable() {}

    Transform::~Transform() {}

    unsigned int Transform::getComponentType()
    {
        return COMPONENT_TRANSFORM;
    }
}