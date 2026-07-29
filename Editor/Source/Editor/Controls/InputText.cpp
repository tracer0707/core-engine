#include "InputText.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

namespace Editor
{
	InputText::InputText() : Control() {}

	InputText::~InputText() {}

	void InputText::update()
	{
		if (!_visible) return;

		const ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.35f);
		}

		ImGui::SetNextItemWidth(_width);
		ImGui::InputText((std::string("##") + _id).c_str(), &_text);

		_actualWidth = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
		_actualHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;

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