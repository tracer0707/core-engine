#pragma once

#include "Control.h"

namespace Editor
{
    class Layout : public Control
    {
      protected:
        float _totalWidth = FLT_MAX;
        float _totalHeight = FLT_MAX;

        Layout();
        virtual ~Layout() = default;

      public:
        virtual float getWidth() override;
        virtual void setWidth(float value) override;

        virtual float getHeight() override;
        virtual void setHeight(float value) override;
    };
} // namespace Editor