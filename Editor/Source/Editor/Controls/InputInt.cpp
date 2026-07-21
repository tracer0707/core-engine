#include "InputInt.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	InputInt::InputInt() {}

	InputInt::~InputInt() {}

	void InputInt::update()
	{
		if (!_visible) return;

		_actualWidth = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
		_actualHeight = ImGui::GetFrameHeightWithSpacing();
	}
} // namespace Editor