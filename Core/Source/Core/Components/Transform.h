#pragma once

#include "Component.h"

#include "../Shared/String.h"
#include "../Interface/Transformable.h"

namespace Core
{
    class Object;

    class Transform : public Component, public Transformable
    {
        friend class Object;

      private:
        Transform(Object* owner);
        virtual ~Transform();

      public:
		virtual ComponentType getComponentType() const { return ComponentType::Transform; }
    };
} // namespace Core