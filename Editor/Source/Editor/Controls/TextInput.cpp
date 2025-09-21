#include "TextInput.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <Core/System/EventHandler.h>

#include "ControlList.h"

namespace Editor
{
	TextInput::TextInput() : Control()
	{
	}

	TextInput::~TextInput()
	{
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

		ImGui::InputText("", &_text);

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