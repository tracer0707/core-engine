#include "ContextMenu.h"

#include <imgui.h>

#include "ControlList.h"

namespace Editor
{
	ContextMenu::ContextMenu()
	{
		_style.paddingX = 5;
		_style.paddingY = 5;
	}

	ContextMenu::~ContextMenu() {}

	int ContextMenu::getControlType() const
	{
		return CONTROL_CONTEXT_MENU;
	}

	void ContextMenu::update()
	{
		if (!_visible) return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(_style.paddingX, _style.paddingY));
		if (ImGui::BeginPopup(_id.c_str()))
		{

			for (int i = 0; i < (int)_controls.count(); ++i)
			{
				Control* c = _controls[i];
				c->update();
			}

			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}

	void ContextMenu::open()
	{
		ImGui::OpenPopup(_id.c_str());
	}
} // namespace Editor