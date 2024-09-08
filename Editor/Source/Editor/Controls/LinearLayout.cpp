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

	void AlignItems(float width, float height, float halignment = 0.0f, float valignment = 0.0f)
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
		if (_hAlignment != LayoutAlignment::Start || _vAlignment != LayoutAlignment::Start)
		{
			float prevX = ImGui::GetCursorPosX();
			float prevY = ImGui::GetCursorPosY();

			float width = 0;
			float height = 0;

			ImGui::SetCursorPosX(-10000);
			ImGui::SetCursorPosY(-10000);

			int idx = 1;

			for (auto it : _controls)
			{
				it->update();
				ImVec2 sz = ImGui::GetItemRectSize();
				width += sz.x;
				height += sz.y;

				if (idx < _controls.count())
				{
					width += ImGui::GetStyle().ItemSpacing.x;
					height += ImGui::GetStyle().ItemSpacing.y;
				}

				++idx;
			}

			ImGui::SetCursorPosX(prevX);
			ImGui::SetCursorPosY(prevY);

			float alignH = _hAlignment == LayoutAlignment::Start ? 0.0f : _hAlignment == LayoutAlignment::Center ? 0.5f : 1.0f;
			float alignV = _vAlignment == LayoutAlignment::Start ? 0.0f : _vAlignment == LayoutAlignment::Center ? 0.5f : 1.0f;

			AlignItems(width, height, alignH, alignV);
		}

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
	}
}