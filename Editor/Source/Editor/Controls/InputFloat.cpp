#include "InputFloat.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	InputFloat::InputFloat() {}

	InputFloat::~InputFloat() {}

	void InputFloat::measure() const
	{
		const ImGuiStyle& style = ImGui::GetStyle();
		_actualWidth = (_width > 0.0f) ? _width : 100.0f;
		_actualHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
	}

	void InputFloat::update()
	{
		if (!_visible) return;

		const ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.35f);
		}

		float _w = getWidth();

		if (_step > 0)
		{
			_w -= (40.0f + (style.ItemSpacing.x * 2.0f));
		}

		ImGui::SetNextItemWidth(_w);
		bool changed = ImGui::InputFloat((std::string("##") + _id).c_str(), &_value, 0.0f, 0.0f, "%.4f");

		if (_step > 0)
		{
			ImGui::SameLine();

			if (ImGui::Button("-", ImVec2(20.0f, 0.0f)))
			{
				if (_incrementType == IncrementType::Additive)
					_value -= _step;
				else
					_value /= _step;

				changed = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("+", ImVec2(20.0f, 0.0f)))
			{
				if (_incrementType == IncrementType::Additive)
					_value += _step;
				else
					_value *= _step;

				changed = true;
			}
		}

		if (changed)
		{
			if (_value < _limitMin) _value = _limitMin;
			if (_value > _limitMax) _value = _limitMax;
		}

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