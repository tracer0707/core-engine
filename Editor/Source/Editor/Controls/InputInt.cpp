#include "InputInt.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	InputInt::InputInt() {}

	InputInt::~InputInt() {}

	float InputInt::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float InputInt::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	void InputInt::update()
	{
		if (!_visible) return;

		ImVec2 _actualSize = ImGui::GetItemRectSize();
		_actualWidth = _actualSize.x;
		_actualHeight = _actualSize.y;
	}
} // namespace Editor