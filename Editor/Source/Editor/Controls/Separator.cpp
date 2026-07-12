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
		if (_direction == SeparatorDirection::Horizontal)
		{
			ImGui::Separator();
		}
		else
		{
			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		}
	}
}