#include "Transform.h"

namespace Core
{
    Transform::Transform(Object* owner) : Component(owner), Transformable() {}

    Transform::~Transform() {}
}