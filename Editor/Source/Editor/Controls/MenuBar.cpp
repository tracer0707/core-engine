#include "MenuBar.h"

#include <imgui.h>

namespace Editor
{
	MenuBar::MenuBar()
	{
	}

	MenuBar::~MenuBar()
	{
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
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Create"))
				{
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
				}
				if (ImGui::MenuItem("Save as.."))
				{
				}
				ImGui::EndMenu();
			}

			if (_isMainMenu)
				ImGui::EndMainMenuBar();
			else
				ImGui::EndMenuBar();
		}
	}
}