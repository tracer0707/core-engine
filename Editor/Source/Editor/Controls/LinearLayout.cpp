#include "LinearLayout.h"

#include <imgui.h>

namespace Editor
{
	LinearLayout::LinearLayout() : Control() {}

	LinearLayout::LinearLayout(LayoutDirection direction) : Control()
	{
		_direction = direction;
	}

	LinearLayout::~LinearLayout() {}

	static void AlignItems(float width, float height, float halignment = 0.0f, float valignment = 0.0f)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		float availX = ImGui::GetContentRegionAvail().x;
		float availY = ImGui::GetContentRegionAvail().y;

		float offX = (availX - width) * halignment;
		float offY = (availY - height) * valignment;

		if (offX > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offX);
		if (offY > 0.0f) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offY);
	}

	void LinearLayout::update()
	{
		if (!_visible) return;

		if (_hAlignment != LayoutAlignment::Start || _vAlignment != LayoutAlignment::Start)
		{
			if (_totalWidth != FLT_MAX && _totalHeight != FLT_MAX)
			{
				float alignH = _hAlignment == LayoutAlignment::Start ? 0.0f : _hAlignment == LayoutAlignment::Center ? 0.5f : 1.0f;
				float alignV = _vAlignment == LayoutAlignment::Start ? 0.0f : _vAlignment == LayoutAlignment::Center ? 0.5f : 1.0f;

				AlignItems(_totalWidth, _totalHeight, alignH, alignV);
			}
		}

		ImGui::BeginChild(_id.c_str(), ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
		for (auto it : _controls)
		{
			it->update();

			if (_direction == LayoutDirection::Horizontal)
			{
				ImGui::SameLine();
			}
			else if (_controls.count() > 1)
			{
				ImGui::SameLine();
				ImGui::NewLine();
			}
		}
		ImGui::EndChild();

		ImVec2 groupSize = ImGui::GetItemRectSize();
		_totalWidth = groupSize.x;
		_totalHeight = groupSize.y;
	}
}