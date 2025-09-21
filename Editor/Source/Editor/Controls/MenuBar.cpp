#include "MenuBar.h"

#include <imgui.h>

#include <Core/System/EventHandler.h>

#include "ControlList.h"
#include "MenuItem.h"

namespace Editor
{
	MenuBar::MenuBar() : Control() {}

	MenuBar::~MenuBar() {}

	void MenuBar::update()
	{
		bool _menu = false;
		if (_isMainMenu)
			_menu = ImGui::BeginMainMenuBar();
		else
			_menu = ImGui::BeginMenuBar();

		if (_menu)
		{
			_height = ImGui::GetWindowHeight();

			for (auto it : _controls)
			{
				it->update();
			}

			if (_isMainMenu)
				ImGui::EndMainMenuBar();
			else
				ImGui::EndMenuBar();
		}
	}

	int MenuBar::getControlType()
	{
		return CONTROL_MENU_BAR;
	}
}