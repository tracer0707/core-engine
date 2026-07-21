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

    void Label::update()
	{
		if (!_visible) return;

		std::string _str = _text.std_str();
		ImGui::Text(_str.c_str());

		ImVec2 textSize = ImGui::CalcTextSize(_text.std_str().c_str());

		_actualWidth = textSize.x;
		_actualHeight = textSize.y;
	}
}