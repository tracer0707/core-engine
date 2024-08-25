#include "Button.h"

#include <imgui.h>

namespace Editor
{
	Button::Button(): Control()
	{
	}

	Button::~Button()
	{
	}

	void Button::update()
	{
		ImGui::Button(ToStdString(text).c_str());
	}
}