#pragma once

#include "Layout.h"

#include <vector>
#include <imgui.h>

namespace Editor
{
    enum class LayoutDirection
    {
        Horizontal,
        Vertical,
    };

    enum class LayoutHorizontalAlignment
    {
        Left,
        Center,
        Right
    };

    enum class LayoutVerticalAlignment
    {
        Top,
        Middle,
        Bottom
    };

    enum class LayoutWrapMode
    {
        NoWrap,
        Wrap,
        WrapReverse
    };

    class LinearLayout : public Layout
    {
      private:
        LayoutDirection _direction = LayoutDirection::Horizontal;
        LayoutHorizontalAlignment _horizontalAlign = LayoutHorizontalAlignment::Left;
        LayoutVerticalAlignment _verticalAlign = LayoutVerticalAlignment::Top;
        LayoutWrapMode _wrapMode = LayoutWrapMode::Wrap;

        ImVec2 _scrollOffset = ImVec2(0.0f, 0.0f);
        float _scrollbarWidth = 8.0f;
        float _actualWidth = 0.0f;
        float _actualHeight = 0.0f;

        struct RowInfo
        {
            std::vector<size_t> indices;
            float totalWidth = 0.0f;
            float maxHeight = 0.0f;
            float startY = 0.0f;
        };

        struct ColumnInfo
        {
            std::vector<size_t> indices;
            float totalHeight = 0.0f;
            float maxWidth = 0.0f;
            float startX = 0.0f;
        };

        void calculateSizes(std::vector<ImVec2>& sizes);
        void calculateWrappedRows(float availableWidth, const std::vector<ImVec2>& sizes, std::vector<RowInfo>& rows);
        void calculateWrappedColumns(float availableHeight, const std::vector<ImVec2>& sizes, std::vector<ColumnInfo>& columns);
        void updateHorizontalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes);
        void updateHorizontalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes);
        void updateVerticalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes);
        void updateVerticalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes);

      public:
        LinearLayout();
        LinearLayout(LayoutDirection direction);
        virtual ~LinearLayout();

        virtual float getWidth() const;
        virtual float getHeight() const;

        virtual int getControlType() const;
        virtual void update();

        void setDirection(LayoutDirection value) { _direction = value; }
        LayoutDirection getDirection() const { return _direction; }

        void setHorizontalAlignment(LayoutHorizontalAlignment align) { _horizontalAlign = align; }
        LayoutHorizontalAlignment getHorizontalAlignment() const { return _horizontalAlign; }

        void setVerticalAlignment(LayoutVerticalAlignment align) { _verticalAlign = align; }
        LayoutVerticalAlignment getVerticalAlignment() const { return _verticalAlign; }

        void setWrapMode(LayoutWrapMode mode) { _wrapMode = mode; }
        LayoutWrapMode getWrapMode() const { return _wrapMode; }
    };
} // namespace Editor