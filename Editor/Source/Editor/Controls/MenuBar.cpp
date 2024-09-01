#include "MenuBar.h"

#include <imgui.h>

namespace Editor
{
	MenuBar::MenuBar()
	{
	}

	MenuBar::~MenuBar()
	{
		for (auto it : _menuItems)
		{
			delete it;
		}

		_menuItems.clear();
	}

	MenuItem::~MenuItem()
	{
		for (auto it : _menuItems)
		{
			delete it;
		}

		_menuItems.clear();
	}

	void MenuItem::update()
	{
		if (_text == "-")
		{
			ImGui::Separator();
			return;
		}

		if (_menuItems.count() > 0)
		{
			if (ImGui::BeginMenu(ToStdString(_text).c_str()))
			{
				for (auto it : _menuItems)
				{
					it->update();
				}

				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::MenuItem(ToStdString(_text).c_str()))
			{
				if (_onClick != nullptr) _onClick();
			}
		}
	}

	void MenuBar::update()
	{
		bool _menu = false;
		if (_isMainMenu)
			_menu = ImGui::BeginMainMenuBar();
		else
			_menu = ImGui::BeginMenuBar();

		if (_menu)
		{
			for (auto it : _menuItems)
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