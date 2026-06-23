#include "InputText.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

namespace Editor
{
	InputText::InputText() : Control() {}

	InputText::~InputText() {}

    float InputText::getWidth() const
    {
        if (_width == 0.0f)
        {
            return _actualWidth;
        }

        return _width;
    }

    float InputText::getHeight() const
    {
        if (_height == 0.0f)
        {
            return _actualHeight;
        }

        return _height;
    }

	void InputText::update()
	{
		if (!_visible) return;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
		}

		ImGui::InputText((std::string("##") + _id).c_str(), &_text);

		ImVec2 _actualSize = ImGui::GetItemRectSize();
        _actualWidth = _actualSize.x;
        _actualHeight = _actualSize.y;

		if (_text != _prevText && _onValueChanged != nullptr)
		{
			_onValueChanged(_text);
			_prevText = _text;
		}

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}