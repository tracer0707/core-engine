#include "MenuItem.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "../../Resources/Texture.h"

namespace Editor
{
	MenuItem::MenuItem(Core::String text) : Control()
	{
		_text = text;
	}

	MenuItem::~MenuItem() {}

	void MenuItem::measure() const
	{
		_actualWidth = ImGui::CalcTextSize(_text.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
		_actualHeight = ImGui::GetFrameHeight();
	}

	void MenuItem::update()
	{
		constexpr float iconSize = 16.0f;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		Core::String text = _text;
		if (_icon != nullptr)
		{
			text = Core::String("       ") + text;
		}

		if (_controls.count() > 0)
		{
			bool opened = ImGui::BeginMenu(text.c_str());

			const ImRect rect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

			if (_icon != nullptr)
			{
				window->DrawList->AddImage((ImTextureID)_icon->getNativeId(),
										   ImVec2(rect.Min.x + 4.0f, rect.Min.y + (rect.GetHeight() - iconSize) * 0.5f),
										   ImVec2(rect.Min.x + 4.0f + iconSize, rect.Min.y + (rect.GetHeight() + iconSize) * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
			}

			if (opened)
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
			bool click = ImGui::MenuItem(text.c_str());
			
			const ImRect rect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

			if (_icon != nullptr)
			{
				window->DrawList->AddImage((ImTextureID)_icon->getNativeId(),
										   ImVec2(rect.Min.x + 4.0f, rect.Min.y + (rect.GetHeight() - iconSize) * 0.5f),
										   ImVec2(rect.Min.x + 4.0f + iconSize, rect.Min.y + (rect.GetHeight() + iconSize) * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
			}

			if (click)
			{
				if (_onClick != nullptr) _onClick();
			}
		}
	}
}