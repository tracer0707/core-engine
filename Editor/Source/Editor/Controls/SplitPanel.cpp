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

    float SplitPanel::getWidth() const
    {
        if (_width == 0.0f)
        {
            return _actualWidth;
        }

        return _width;
    }

    float SplitPanel::getHeight() const
    {
        if (_height == 0.0f)
        {
            return _actualHeight;
        }

        return _height;
    }

    int SplitPanel::getControlType() const
    {
        return CONTROL_SPLITTER;
    }

    void SplitPanel::update()
    {
        if (!_visible) return;

        ImGuiContext& g = *ImGui::GetCurrentContext();
        ImGuiStyle& style = ImGui::GetStyle();
        
        ImGui::BeginChild(_id.c_str(), ImVec2(_width, _height));

        ImGuiWindow* window = g.CurrentWindow;

        float splitter_width = 2.0f;

        bool horizontal = _direction == SplitPanelDirection::Horizontal;

        float size2 = 0;

        for (int i = 0; i < _controls.count(); ++i)
        {
            Control* control1 = _controls.get(i);
            control1->update();

            float size1 = 0;

            if (horizontal)
            {
                size1 = control1->getWidth();
                ImGui::SameLine();
            }
            else
            {
                size1 = control1->getHeight();
            }

            if (i + 1 < _controls.count())
            {
                Control* control2 = _controls.get(i + 1);

                if (horizontal)
                    size2 = control2->getWidth();
                else
                    size2 = control2->getHeight();
            }
            else
            {
                break;
            }

            ImRect splitter_bb;
            
            if (horizontal)
            {
                splitter_bb = ImRect(window->DC.CursorPos.x - style.FramePadding.x, window->DC.CursorPos.y,
                                     window->DC.CursorPos.x - style.FramePadding.x + splitter_width,
                                     window->DC.CursorPos.y + _actualHeight);
            }
            else
            {
                splitter_bb = ImRect(window->DC.CursorPos.x - style.FramePadding.x, window->DC.CursorPos.y,
                                     window->DC.CursorPos.x - style.FramePadding.x + _actualWidth,
                                     window->DC.CursorPos.y + splitter_width);
            }

            ImGuiID splitter_id = window->GetID((_id + "_splitter_" + std::to_string(i)).c_str());
            ImGui::SplitterBehavior(splitter_bb, splitter_id, horizontal ? ImGuiAxis_X : ImGuiAxis_Y, &size1, &size2, 30, 30);

            Control* control2 = _controls.get(i + 1);
            if (i + 1 == _controls.count() - 1)
            {
                if (horizontal)
                    size2 = window->SizeFull.x - size1 - style.FramePadding.x;
                else
                    size2 = window->SizeFull.y - size1 - style.FramePadding.y;
            }

            if (horizontal)
            {
                control1->setWidth(size1);
                control2->setWidth(size2);
            }
            else
            {
                control1->setHeight(size1);
                control2->setHeight(size2);
            }

            if (horizontal) ImGui::SameLine();
        }

        ImGui::EndChild();

        ImVec2 _actualSize = ImGui::GetWindowSize();
        _actualWidth = _actualSize.x;
        _actualHeight = _actualSize.y;
    }
} // namespace Editor