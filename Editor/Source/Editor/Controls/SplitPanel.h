#pragma once

#include "Control.h"
#include <Core/Shared/List.h>

namespace Editor
{
    enum class SplitPanelDirection
    {
        Horizontal,
        Vertical,
    };

    class SplitPanel : public Control
    {
      private:
        SplitPanelDirection _direction = SplitPanelDirection::Vertical;
        Core::List<float> _sizeMap;

        float _startSize = 100;
        float _prevWidth = 0;
        float _prevHeight = 0;

      public:
        SplitPanel();
        SplitPanel(SplitPanelDirection direction);
        virtual ~SplitPanel();

        void setStartSize(float value) { _startSize = value; }

        virtual int getControlType();
        virtual void update();
    };
} // namespace Editor