#include "Separator.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Separator::Separator() : Control() {}

	Separator::Separator(SeparatorDirection direction) : Control()
	{
		_direction = direction;
	}

	Separator::~Separator() {}

	void Separator::measure() const
	{
		if (_direction == SeparatorDirection::Horizontal)
		{
			_actualWidth = _width > 0.0f ? _width : ImGui::GetContentRegionAvail().x;
			_actualHeight = 1.0f;
		}
		else
		{
			_actualWidth = 1.0f;
			_actualHeight = _height > 0.0f ? _height : ImGui::GetTextLineHeightWithSpacing();
		}
	}

	void Separator::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();

		if (_direction == SeparatorDirection::Horizontal)
		{
			ImGui::Separator();
		}
		else
		{
			ImVec2 p = ImGui::GetCursorScreenPos();
			float line_height = getHeight();

			ImGui::GetWindowDrawList()->AddLine(p, ImVec2(p.x, p.y + line_height), ImGui::GetColorU32(ImGuiCol_Separator));

			ImGui::Dummy(ImVec2(1.0f, line_height));
			
		}
	}
}