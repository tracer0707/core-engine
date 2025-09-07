#include "TextInput.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <System/EventHandler.h>

#include "ControlList.h"

namespace Editor
{
	TextInput::TextInput()
	{
	}

	TextInput::~TextInput()
	{
	}

	UString TextInput::getText()
	{
		return _text;
	}

	void TextInput::setText(UString value)
	{
		_text = value;
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

		ImGui::InputText("", _buffer, IM_ARRAYSIZE(_buffer));

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