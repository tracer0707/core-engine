#include "Checkbox.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Checkbox::Checkbox() {}

	Checkbox::~Checkbox() {}

	void Checkbox::measure() const
	{
		_actualWidth = (_width > 0.0f) ? _width : ImGui::GetFrameHeight();
		_actualHeight = ImGui::GetFrameHeight();
	}

	void Checkbox::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.35f);
		}

		bool changed = ImGui::Checkbox((std::string("##") + _id).c_str(), &_value);

		if (changed && _onValueChanged != nullptr)
		{
			_onValueChanged(_value);
		}

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
} // namespace Editor