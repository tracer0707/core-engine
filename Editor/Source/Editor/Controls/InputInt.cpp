#include "InputInt.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	InputInt::InputInt() {}

	InputInt::~InputInt() {}

	void InputInt::update()
	{
		if (!_visible) return;

		const ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.35f);
		}

		ImGui::SetNextItemWidth(_actualWidth);
		ImGui::InputInt((std::string("##") + _id).c_str(), &_value, _step);

		_actualWidth = (_width > 0.0f) ? _width : 100.0f;
		_actualHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;

		if (_value != _prevValue && _onValueChanged != nullptr)
		{
			_onValueChanged(_value);
			_prevValue = _value;
		}

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
} // namespace Editor