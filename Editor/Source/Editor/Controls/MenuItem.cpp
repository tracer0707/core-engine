#include "MenuItem.h"

#include <imgui.h>

#include "ControlList.h"

namespace Editor
{
	MenuItem::~MenuItem() {}

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

	int MenuItem::getControlType()
	{
		return CONTROL_MENU_ITEM;
	}
}