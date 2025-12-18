#include "LinearLayout.h"

#include <imgui_internal.h>

#include "ControlList.h"

namespace Editor
{
    LinearLayout::LinearLayout() : Layout() {}

    LinearLayout::LinearLayout(LayoutDirection direction) : Layout()
    {
        _direction = direction;
    }

    LinearLayout::~LinearLayout() {}

    float LinearLayout::getWidth() const
    {
        if (_width == 0.0f)
        {
            return _actualWidth;
        }

        return _width;
    }

    float LinearLayout::getHeight() const
    {
        if (_height == 0.0f)
        {
            return _actualHeight;
        }

        return _height;
    }

    void LinearLayout::calculateSizes(std::vector<ImVec2>& sizes)
    {
        sizes.clear();
        sizes.reserve(_controls.count());

        for (auto& control : _controls)
        {
            sizes.push_back(ImVec2(control->getWidth(), control->getHeight()));
        }
    }

    void LinearLayout::calculateWrappedRows(float availableWidth, const std::vector<ImVec2>& sizes, std::vector<RowInfo>& rows)
    {
        rows.clear();

        float currentX = 0.0f;
        RowInfo currentRow;

        ImGuiStyle& style = ImGui::GetStyle();

        for (size_t i = 0; i < sizes.size(); ++i)
        {
            const ImVec2& size = sizes[i];

            if (currentRow.indices.empty() || (currentRow.totalWidth + size.x + (currentRow.indices.empty() ? 0 : style.ItemSpacing.x) <= availableWidth))
            {
                if (!currentRow.indices.empty())
                {
                    currentRow.totalWidth += style.ItemSpacing.x;
                }
                currentRow.indices.push_back(i);
                currentRow.totalWidth += size.x;
                currentRow.maxHeight = std::max(currentRow.maxHeight, size.y);
            }
            else
            {
                if (!currentRow.indices.empty())
                {
                    rows.push_back(currentRow);
                }

                currentRow = RowInfo();
                currentRow.indices.push_back(i);
                currentRow.totalWidth = size.x;
                currentRow.maxHeight = size.y;
            }
        }

        if (!currentRow.indices.empty())
        {
            rows.push_back(currentRow);
        }
    }

    void LinearLayout::calculateWrappedColumns(float availableHeight, const std::vector<ImVec2>& sizes, std::vector<ColumnInfo>& columns)
    {
        columns.clear();

        float currentY = 0.0f;
        ColumnInfo currentColumn;

        ImGuiStyle& style = ImGui::GetStyle();

        for (size_t i = 0; i < sizes.size(); ++i)
        {
            const ImVec2& size = sizes[i];

            if (currentColumn.indices.empty() ||
                (currentColumn.totalHeight + size.y + (currentColumn.indices.empty() ? 0 : style.ItemSpacing.y) <= availableHeight))
            {
                if (!currentColumn.indices.empty())
                {
                    currentColumn.totalHeight += style.ItemSpacing.y;
                }

                currentColumn.indices.push_back(i);
                currentColumn.totalHeight += size.y;
                currentColumn.maxWidth = std::max(currentColumn.maxWidth, size.x);
            }
            else
            {
                if (!currentColumn.indices.empty())
                {
                    columns.push_back(currentColumn);
                }

                currentColumn = ColumnInfo();
                currentColumn.indices.push_back(i);
                currentColumn.totalHeight = size.y;
                currentColumn.maxWidth = size.x;
            }
        }

        if (!currentColumn.indices.empty())
        {
            columns.push_back(currentColumn);
        }
    }

    void LinearLayout::updateHorizontalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes)
    {
        float totalWidth = 0.0f;
        float maxHeight = 0.0f;

        for (const auto& size : sizes)
        {
            totalWidth += size.x;
            maxHeight = std::max(maxHeight, size.y);
        }

        ImGuiStyle& style = ImGui::GetStyle();

        if (_controls.count() > 1)
        {
            totalWidth += style.ItemSpacing.x * (_controls.count() - 1);
        }

        float startX = startPos.x;
        if (_horizontalAlign == LayoutHorizontalAlignment::Center)
        {
            startX += (availableSize.x - totalWidth) * 0.5f;
        }
        else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
        {
            startX += availableSize.x - totalWidth;
        }

        float currentX = startX;

        for (size_t i = 0; i < _controls.count(); ++i)
        {
            Control* c = _controls[i];
            const ImVec2& size = sizes[i];

            float y = startPos.y;
            if (_verticalAlign == LayoutVerticalAlignment::Middle)
            {
                y += (maxHeight - size.y) * 0.5f;
            }
            else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
            {
                y += maxHeight - size.y;
            }

            ImGui::SetCursorPos(ImVec2(currentX, y));

            c->update();

            currentX += size.x + style.ItemSpacing.x;
        }

        //ImGui::SetCursorPos(ImVec2(startPos.x, startPos.y + maxHeight + style.FramePadding.y));
    }

    void LinearLayout::updateHorizontalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes)
    {
        std::vector<RowInfo> rows;
        calculateWrappedRows(availableSize.x, sizes, rows);

        ImGuiStyle& style = ImGui::GetStyle();

        float currentY = startPos.y;

        for (size_t rowIdx = 0; rowIdx < rows.size(); ++rowIdx)
        {
            RowInfo& row = rows[rowIdx];
            row.startY = currentY;

            float startX = startPos.x;
            if (_horizontalAlign == LayoutHorizontalAlignment::Center)
            {
                startX += (availableSize.x - row.totalWidth) * 0.5f;
            }
            else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
            {
                startX += availableSize.x - row.totalWidth;
            }

            float currentX = startX;

            for (size_t idx : row.indices)
            {
                Control* c = _controls[idx];
                const ImVec2& size = sizes[idx];

                float y = row.startY;
                if (_verticalAlign == LayoutVerticalAlignment::Middle)
                {
                    y += (row.maxHeight - size.y) * 0.5f;
                }
                else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
                {
                    y += row.maxHeight - size.y;
                }

                ImGui::SetCursorPos(ImVec2(currentX, y));
                c->update();

                currentX += size.x + style.ItemSpacing.x;
            }

            currentY += row.maxHeight + style.ItemSpacing.y;
        }

        //if (!rows.empty())
        //{
            //ImGui::SetCursorPos(ImVec2(startPos.x, currentY + style.FramePadding.y));
        //}
    }

    void LinearLayout::updateVerticalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes)
    {
        float totalHeight = 0.0f;
        float maxWidth = 0.0f;

        for (const auto& size : sizes)
        {
            totalHeight += size.y;
            maxWidth = std::max(maxWidth, size.x);
        }

        ImGuiStyle& style = ImGui::GetStyle();

        if (_controls.count() > 1)
        {
            totalHeight += style.ItemSpacing.y * (_controls.count() - 1);
        }

        float startY = startPos.y;
        if (_verticalAlign == LayoutVerticalAlignment::Middle)
        {
            startY += (availableSize.y - totalHeight) * 0.5f;
        }
        else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
        {
            startY += availableSize.y - totalHeight;
        }

        float currentY = startY;

        for (size_t i = 0; i < _controls.count(); ++i)
        {
            Control* c = _controls[i];
            const ImVec2& size = sizes[i];

            float x = startPos.x;
            if (_horizontalAlign == LayoutHorizontalAlignment::Center)
            {
                x += (maxWidth - size.x) * 0.5f;
            }
            else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
            {
                x += maxWidth - size.x;
            }

            ImGui::SetCursorPos(ImVec2(x, currentY));

            c->update();

            currentY += size.y + style.ItemSpacing.y;
        }

        //ImGui::SetCursorPos(ImVec2(startPos.x, currentY + style.FramePadding.y));
    }

    void LinearLayout::updateVerticalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes)
    {
        std::vector<ColumnInfo> columns;
        calculateWrappedColumns(availableSize.y, sizes, columns);

        ImGuiStyle& style = ImGui::GetStyle();

        float currentX = startPos.x;

        for (size_t colIdx = 0; colIdx < columns.size(); ++colIdx)
        {
            ColumnInfo& col = columns[colIdx];
            col.startX = currentX;

            float startY = startPos.y;
            if (_verticalAlign == LayoutVerticalAlignment::Middle)
            {
                startY += (availableSize.y - col.totalHeight) * 0.5f;
            }
            else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
            {
                startY += availableSize.y - col.totalHeight;
            }

            float currentY = startY;

            for (size_t idx : col.indices)
            {
                Control* c = _controls[idx];
                const ImVec2& size = sizes[idx];

                float x = col.startX;
                if (_horizontalAlign == LayoutHorizontalAlignment::Center)
                {
                    x += (col.maxWidth - size.x) * 0.5f;
                }
                else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
                {
                    x += col.maxWidth - size.x;
                }

                ImGui::SetCursorPos(ImVec2(x, currentY));
                c->update();

                currentY += size.y + style.ItemSpacing.y;
            }

            currentX += col.maxWidth + style.ItemSpacing.x;
        }

        /*if (!columns.empty())
        {
            ImGui::SetCursorPos(ImVec2(currentX + style.FramePadding.x, startPos.y));
        }*/
    }

    void LinearLayout::update()
    {
        if (!_visible) return;

        uint32_t _flags = ImGuiChildFlags_AlwaysUseWindowPadding;
        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(_style.paddingX, _style.paddingY));
        ImGui::BeginChild(_id.c_str(), ImVec2(_width, _height), _flags);

        ImVec2 availableSize = ImGui::GetContentRegionAvail();

        //availableSize.x -= (style.FramePadding.x * 2.0f);
        //availableSize.y -= (style.FramePadding.y * 2.0f);

        ImVec2 cursorStart = ImGui::GetCursorPos();
        
        std::vector<ImVec2> sizes;
        calculateSizes(sizes);

        if (_direction == LayoutDirection::Horizontal)
        {
            if (_wrapMode == LayoutWrapMode::NoWrap)
            {
                updateHorizontalLayout(cursorStart, availableSize, sizes);
            }
            else
            {
                updateHorizontalLayoutWrapped(cursorStart, availableSize, sizes);
            }
        }
        else
        {
            if (_wrapMode == LayoutWrapMode::NoWrap)
            {
                updateVerticalLayout(cursorStart, availableSize, sizes);
            }
            else
            {
                updateVerticalLayoutWrapped(cursorStart, availableSize, sizes);
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImVec2 _actualSize = ImGui::GetWindowSize();
        _actualWidth = _actualSize.x;
        _actualHeight = _actualSize.y;
    }

    int LinearLayout::getControlType() const
    {
        return CONTROL_LINEAR_LAYOUT;
    }
} // namespace Editor