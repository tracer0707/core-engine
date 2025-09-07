#include "Separator.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ControlList.h"

namespace Editor
{
	Separator::Separator() {}

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

	int Separator::getControlType()
	{
		return CONTROL_SEPARATOR;
	}
}