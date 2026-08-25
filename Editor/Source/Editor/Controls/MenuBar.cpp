#include "MenuBar.h"

#include <imgui.h>

#include <Core/System/EventHandler.h>

#include "MenuItem.h"

namespace Editor
{
	MenuBar::MenuBar() : Control() {}

	MenuBar::~MenuBar() {}

	void MenuBar::measure() const
	{
		_actualHeight = ImGui::GetFrameHeight();
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