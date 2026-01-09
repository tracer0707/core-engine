#include "ResourceSelect.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ControlList.h"

namespace Editor
{
	ResourceSelect::ResourceSelect() : Control() {}

	ResourceSelect::~ResourceSelect()
	{

	}

	float ResourceSelect::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float ResourceSelect::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	int ResourceSelect::getControlType() const
	{
		return CONTROL_RESOURCE_SELECT;
	}

	void ResourceSelect::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 total_size(_width > 0 ? _width : -1, _height > 0 ? _height : 24.0f);

		ImGui::PushID(_id.c_str());
		bool hasClick = ImGui::InvisibleButton("##ResourceSelect", total_size);
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 size = ImGui::GetItemRectSize();
		ImGui::PopID();

		ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_col, style.FrameRounding);

		ImVec2 _actualSize = ImGui::GetItemRectSize();
		_actualWidth = _actualSize.x;
		_actualHeight = _actualSize.y;
	}
} // namespace Editor