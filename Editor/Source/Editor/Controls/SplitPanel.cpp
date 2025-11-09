#include "SplitPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Layout.h"
#include "ControlList.h"

namespace Editor
{
    SplitPanel::SplitPanel() : Control() {}

    SplitPanel::SplitPanel(SplitPanelDirection direction)
    {
        _direction = direction;
    }

    SplitPanel::~SplitPanel() {}

    int SplitPanel::getControlType()
    {
        return CONTROL_SPLITTER;
    }

    void SplitPanel::update()
    {
        if (!_visible) return;

        ImGuiContext& g = *ImGui::GetCurrentContext();

        float splitter_width = 2.0f;

        bool horizontal = _direction == SplitPanelDirection::Horizontal;

        ImGuiWindow* window = g.CurrentWindow;

        ImVec2 sz = ImGui::GetContentRegionAvail();

        bool _new = _sizeMap.count() != _controls.count() && _sizeMap.count() == 0;

        if (_sizeMap.count() != _controls.count())
        {
            _sizeMap.resize(_controls.count());
            _sizeMap.fill(100);
            if (_new && _sizeMap.count() > 0) _sizeMap.set(0, _startSize);
        }

        if (_new && _sizeMap.count() > 1 || sz.x != _prevWidth || sz.y != _prevHeight)
        {
            float size = horizontal ? sz.x : sz.y;
            _sizeMap.set(_sizeMap.count() - 1, size - (100 * (_sizeMap.count() - 1)));
        }

        _prevWidth = sz.x;
        _prevHeight = sz.y;

        for (int i = 0; i < _controls.count(); ++i)
        {
            Control* control1 = _controls.get(i);

            ImRect splitter_bb;

            float size1 = _sizeMap.get(i);
            float size2 = 30;

            if (i + 1 < _sizeMap.count())
            {
                size2 = _sizeMap.get(i + 1);
            }

            if (horizontal)
            {
                splitter_bb = ImRect(window->DC.CursorPos.x + size1, window->DC.CursorPos.y, window->DC.CursorPos.x + size1 + splitter_width,
                                     window->DC.CursorPos.y + window->Size.y);
            }
            else
            {
                splitter_bb = ImRect(window->DC.CursorPos.x, window->DC.CursorPos.y + size1, window->DC.CursorPos.x + window->Size.x,
                                     window->DC.CursorPos.y + size1 + splitter_width);
            }

            if (horizontal)
                control1->setWidth(size1);
            else
                control1->setHeight(size1);

            control1->update();

            if (i == _controls.count() - 1) break;

            if (horizontal) ImGui::SameLine();

            ImGuiID splitter_id = window->GetID((_id + "_split_" + std::to_string(i)).c_str());
            if (ImGui::SplitterBehavior(splitter_bb, splitter_id, horizontal ? ImGuiAxis_X : ImGuiAxis_Y, &size1, &size2, 30, 30, 0.0f))
            {
                _sizeMap.set(i, size1);
                if (i + 1 < _sizeMap.count())
                {
                    _sizeMap.set(i + 1, size2);
                }
            }

            if (horizontal) ImGui::SameLine();
        }
    }
} // namespace Editor