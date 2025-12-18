#include "Button.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Content/Texture.h>

#include "ControlList.h"

namespace Editor
{
    Button::Button() : Control() {}

    Button::Button(Core::String text) : Control()
    {
        _text = text;
    }

    Button::~Button() {}

    float Button::getWidth()
    {
        if (_width == 0.0f)
        {
            return _actualWidth;
        }

        return _width;
    }

    float Button::getHeight()
    {
        if (_height == 0.0f)
        {
            return _actualHeight;
        }

        return _height;
    }

    void Button::update()
    {
        if (!_visible) return;

        bool hasClick = false;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * _style.opacity);

        if (!_style.enabled)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
        }

        if (_image != nullptr)
        {
            int w = _width;
            int h = _height;

            ImGuiStyle& style = ImGui::GetStyle();
            ImVec2 padding = style.FramePadding;

            std::string label = _text.std_str();
            float spacing = !label.empty() ? style.ItemInnerSpacing.y : 0;
            ImVec2 text_size = !label.empty() ? ImGui::CalcTextSize(label.c_str()) : ImVec2(0, 0);

            ImVec2 imgSize(w - padding.x * 2.0f, h - spacing - text_size.y - padding.y * 2.0f);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            if (text_size.x > w) text_size.x = w;
            if (text_size.y > h) text_size.y = h;

            ImVec2 total_size(w, h);

            ImGui::PushID(_id.c_str());
            hasClick = ImGui::InvisibleButton("##ImageButtonWithText", total_size);
            bool hovered = ImGui::IsItemHovered();
            bool active = ImGui::IsItemActive();
            ImVec2 pos = ImGui::GetItemRectMin();
            ImVec2 size = ImGui::GetItemRectSize();
            ImGui::PopID();

            ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

            draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_col, style.FrameRounding);

            ImVec2 img_p(pos.x + (size.x - imgSize.x) * 0.5f, pos.y + padding.y);

            draw_list->AddImage((ImTextureID)_image->getNativeId(), img_p, ImVec2(img_p.x + imgSize.x, img_p.y + imgSize.y), ImVec2(0, 1),
                                ImVec2(1, 0));

            ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);

            float lowSize = (size.x - text_size.x) * 0.5f;
            float highSize = (size.x + text_size.x) * 0.5f;
            ImVec2 text_min(pos.x + lowSize, img_p.y + imgSize.y + spacing);
            ImVec2 text_max(pos.x + highSize, text_min.y + text_size.y);

            ImGui::RenderTextEllipsis(draw_list, text_min, text_max, text_max.x, text_max.x, label.c_str(), nullptr, nullptr);
        }
        else
        {
            hasClick = ImGui::Button(_text.std_str().c_str(), ImVec2(_width, _height));
            ImVec2 _actualSize = ImGui::GetItemRectSize();
            _actualWidth = _actualSize.x;
            _actualHeight = _actualSize.y;
        }

        ImGui::PopStyleVar();

        if (!_style.enabled)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if (hasClick)
        {
            if (_onClick != nullptr)
            {
                _onClick();
            }
        }
    }

    int Button::getControlType()
    {
        return CONTROL_BUTTON;
    }

    void Button::setActive(bool value)
    {
        _active = value;

        if (_active)
            _style.opacity = 1.0f;
        else
            _style.opacity = 0.5f;
    }
} // namespace Editor