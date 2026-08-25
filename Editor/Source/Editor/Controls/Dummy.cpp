#include "Dummy.h"

#include <imgui.h>

namespace Editor
{
	Dummy::Dummy() : Control() {}

	Dummy::~Dummy() {}

	void Dummy::measure() const
	{
		_actualWidth = _width;
		_actualHeight = _height;
	}

	void Dummy::update()
	{
		if (!_visible) return;

		ImVec2 prevPos = ImGui::GetCursorPos();
		ImVec2 newPos = ImGui::GetCursorPos();

		if (_x != FLT_MIN)
		{
			newPos.x = _x;
		}

		if (_y != FLT_MIN)
		{
			newPos.y = _y;
		}

		ImGui::SetCursorPos(newPos);
		ImGui::Dummy(ImVec2(_width, _height));

		updateDragDropTarget();

		ImGui::SetCursorPos(prevPos);

		_computedX = newPos.x;
		_computedY = newPos.y;
	}
} // namespace Editor