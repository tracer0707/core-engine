#include "InputVec4.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	InputVec4::InputVec4() {}

	InputVec4::~InputVec4() {}

	void InputVec4::update()
	{
		if (!_visible) return;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
		}

		ImGui::InputFloat4((std::string("##") + _id).c_str(), &_value[0]);

		_actualWidth = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
		_actualHeight = ImGui::GetFrameHeight();

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