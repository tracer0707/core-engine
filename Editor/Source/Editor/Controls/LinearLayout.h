#pragma once

#include "Layout.h"

namespace Editor
{
    enum class LayoutDirection
    {
        Horizontal,
        Vertical,
    };

    enum class LayoutAlignment
    {
        Start,
        Center,
        End
    };

    class LinearLayout : public Layout
    {
      private:
        LayoutDirection _direction = LayoutDirection::Horizontal;
        LayoutAlignment _hAlignment = LayoutAlignment::Start;
        LayoutAlignment _vAlignment = LayoutAlignment::Start;

        bool _stretchX = false;
        bool _stretchY = false;

      public:
        LinearLayout();
        LinearLayout(LayoutDirection direction);
        virtual ~LinearLayout();

        virtual int getControlType();
        virtual void update();

        void setDirection(LayoutDirection value) { _direction = value; }
        LayoutDirection getDirection() { return _direction; }

        void setHorizontalAlignment(LayoutAlignment value) { _hAlignment = value; }
        LayoutAlignment getHorizontalAlignment() { return _hAlignment; }

        void setVerticalAlignment(LayoutAlignment value) { _vAlignment = value; }
        LayoutAlignment getVerticalAlignment() { return _vAlignment; }

        bool getStretchX() { return _stretchX; }
        void setStretchX(bool value) { _stretchX = value; }

        bool getStretchY() { return _stretchY; }
        void setStretchY(bool value) { _stretchY = value; }
    };
} // namespace Editor