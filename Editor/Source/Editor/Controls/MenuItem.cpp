#include "MenuItem.h"

#include <imgui.h>

namespace Editor
{
	MenuItem::MenuItem(Core::String text) : Control()
	{
		_text = text;
	}

	MenuItem::~MenuItem() {}

	void MenuItem::measure() const
	{
		_actualWidth = ImGui::CalcTextSize(_text.std_str().c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
		_actualHeight = ImGui::GetFrameHeight();
	}

	void MenuItem::update()
	{
		if (_controls.count() > 0)
		{
			if (ImGui::BeginMenu(_text.std_str().c_str()))
			{
				for (auto it : _controls)
				{
					it->update();
				}

				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::MenuItem(_text.std_str().c_str()))
			{
				if (_onClick != nullptr) _onClick();
			}
		}
	}
}