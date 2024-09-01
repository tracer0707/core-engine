#include "MenuBar.h"

#include <imgui.h>

#include <System/EventHandler.h>

#include "MenuItem.h"

namespace Editor
{
	MenuBar::MenuBar() {}

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
}