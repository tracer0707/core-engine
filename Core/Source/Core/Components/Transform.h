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
        virtual unsigned int getComponentType();
    };
} // namespace Core