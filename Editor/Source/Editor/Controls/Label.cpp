#include "Label.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Label::Label() : Control() {}

	Label::Label(Core::String text) : Control()
	{
		_text = text;
	}

	Label::~Label()
	{
	}

	void Label::measure() const
	{
		ImVec2 textSize = ImGui::CalcTextSize(_text.std_str().c_str());
		_actualWidth = textSize.x;
		_actualHeight = textSize.y;
	}

    void Label::update()
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

		std::string _str = _text.std_str();
		ImGui::Text(_str.c_str());

		ImGui::SetCursorPos(prevPos);

		_computedX = newPos.x;
		_computedY = newPos.y;
	}
}