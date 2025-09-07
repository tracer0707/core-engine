#include "Label.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ControlList.h"

namespace Editor
{
	Label::Label() {}

	Label::Label(UString text)
	{
		_text = text;
	}

	Label::~Label()
	{
	}

	void Label::update()
	{
		if (!_visible) return;

		std::string _str = ToStdString(_text);
		ImGui::Text(_str.c_str());
	}

	int Label::getControlType()
	{
		return CONTROL_LABEL;
	}
}