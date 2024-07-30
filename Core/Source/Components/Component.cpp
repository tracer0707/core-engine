#include "Component.h"

namespace Core
{
    Component::Component(Object* owner)
    {
        this->owner = owner;
    }

    Component::~Component()
    {
        owner = nullptr;
    }

    void Component::update(float& dt)
    {

    }

    void Component::render(Camera* camera)
    {

    }
}