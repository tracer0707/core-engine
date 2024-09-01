#include "Button.h"

#include <imgui.h>

namespace Editor
{
	Button::Button() : Control() {}

	Button::Button(UString text) : Control()
	{
		_text = text;
	}

	Button::~Button() {}

	void Button::update()
	{
		if (ImGui::Button(ToStdString(_text).c_str()))
		{
			if (_onClick != nullptr) _onClick();
		}
	}
}