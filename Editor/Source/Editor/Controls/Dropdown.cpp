#include "Dropdown.h"

#include <imgui.h>

#include "ControlList.h"

namespace Editor
{
	Dropdown::Dropdown(Core::List<Core::String> items) : Control()
	{
		_items = items;
	}

	Dropdown::~Dropdown() {}

	float Dropdown::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float Dropdown::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	int Dropdown::getControlType() const
	{
		return CONTROL_DROPDOWN;
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