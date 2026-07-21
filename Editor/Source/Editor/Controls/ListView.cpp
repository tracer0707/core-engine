#include "ListView.h"

#include <imgui.h>
#include <Core/System/EventHandler.h>

namespace Editor
{
    ListView::ListView() : Control()
    {
    }

    ListView::~ListView()
	{
	}

	void ListView::selectItem(Control* value, bool byUser)
	{
		_selectedItems.clear();
		if (value != nullptr)
		{
			_selectedItems.add(value);
		}

		if (byUser && _onSelectionChanged != nullptr)
		{
			_onSelectionChanged(_selectedItems);
		}
	}

	void ListView::clearSelection(bool byUser)
	{
		_selectedItems.clear();

		if (byUser && _onSelectionChanged != nullptr)
		{
			_onSelectionChanged(_selectedItems);
		}
	}

	bool ListView::isItemSelected(Control* node)
	{
		return _selectedItems.contains(node);
	}

	void ListView::update()
	{
        ImGui::BeginChild(_id.c_str(), ImVec2(_width, _height));

		for (auto it : _controls)
		{
			it->update();

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly)) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImVec2 min = ImGui::GetItemRectMin();
				ImVec2 max = ImGui::GetItemRectMax();
				draw_list->AddRectFilled(min, max, ImGui::GetColorU32(ImGuiCol_HeaderHovered));
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				if (_onItemClick != nullptr) _onItemClick(it);
				selectItem(it, true);
			}
		}

		ImGui::EndChild();

		ImGuiStyle& style = ImGui::GetStyle();
		_actualWidth = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;

		if (_height > 0.0f)
		{
			_actualHeight = _height;
		}
		else
		{
			float total_h = 0.0f;
			for (auto it : _controls)
			{
				total_h += it->getHeight();
			}
			
			if (_controls.count() > 1)
				total_h += style.ItemSpacing.y * (_controls.count() - 1);

			_actualHeight = total_h;
		}
	}
}