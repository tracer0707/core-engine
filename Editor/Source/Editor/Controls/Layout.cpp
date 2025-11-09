#include "Layout.h"

namespace Editor
{
    Layout::Layout() : Control() {}

    float Layout::getWidth()
    {
        return _totalWidth;
    }

    void Layout::setWidth(float value)
    {
        _width = value;
        _totalWidth = value;
    }

    float Layout::getHeight()
    {
        return _totalHeight;
    }

    void Layout::setHeight(float value)
    {
        _height = value;
        _totalHeight = value;
    }
}