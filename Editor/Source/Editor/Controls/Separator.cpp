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

	void Separator::update()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		if (_direction == SeparatorDirection::Horizontal)
		{
			ImGui::Separator();
			_actualWidth = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
			_actualHeight = 1.0f;
		}
		else
		{
			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			_actualWidth = 1.0f;
			_actualHeight = (_height > 0.0f) ? _height : ImGui::GetContentRegionAvail().y;
		}
	}
}