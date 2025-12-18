#include "TextInput.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include "ControlList.h"

namespace Editor
{
	TextInput::TextInput() : Control() {}

	TextInput::~TextInput() {}

    float TextInput::getWidth()
    {
        if (_width == 0.0f)
        {
            return _actualWidth;
        }

        return _width;
    }

    float TextInput::getHeight()
    {
        if (_height == 0.0f)
        {
            return _actualHeight;
        }

        return _height;
    }

	Core::String TextInput::getText()
	{
		return _text;
	}

	void TextInput::setText(Core::String value)
	{
		_text = value.std_str();
	}

	void TextInput::update()
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

		if (_text != _prevText && _onTextChanged != nullptr)
		{
			_onTextChanged(_text);
			_prevText = _text;
		}

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}

	int TextInput::getControlType()
	{
		return CONTROL_TEXT_INPUT;
	}
}