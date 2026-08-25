#include "Dropdown.h"

#include <imgui.h>

namespace Editor
{
	Dropdown::Dropdown(Core::List<Core::String> items) : Control()
	{
		_items = items;
	}

	Dropdown::~Dropdown() {}

	void Dropdown::measure() const
	{
		Core::String preview_value = _items[current_item_index];
		ImGuiStyle& style = ImGui::GetStyle();
		_actualWidth = (_width > 0.0f) ? _width : (ImGui::CalcTextSize(preview_value.std_str().c_str()).x + style.FramePadding.x * 2.0f);
		_actualHeight = (_height > 0.0f) ? _height : ImGui::GetFrameHeight();
	}

	void Dropdown::update()
	{
		if (!_visible) return;

		Core::String preview_value = _items[current_item_index];

		if (ImGui::BeginCombo(("##" + _id).c_str(), preview_value.std_str().c_str(), ImGuiComboFlags_None))
		{
			for (int n = 0; n < _items.count(); n++)
			{
				const bool is_selected = (current_item_index == n);
				if (ImGui::Selectable(_items[n].std_str().c_str(), is_selected))
				{
					current_item_index = n;
					if (_onSelectItem != nullptr)
					{
						_onSelectItem(current_item_index);
					}
				}

				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

	}
} // namespace Editor