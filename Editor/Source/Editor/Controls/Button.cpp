#include "Button.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <System/EventHandler.h>
#include <Shared/Guid.h>

#include <Assets/Texture.h>

namespace Editor
{
	Button::Button() : Control()
	{
		_id = FromStdString(Core::Guid::genGuid());
	}

	Button::Button(UString text) : Control()
	{
		_text = text;
		_id = FromStdString(Core::Guid::genGuid());
	}

	Button::~Button() {}

	void Button::update()
	{
		if (!_visible) return;

		bool hasClick = false;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
		}

		if (_image != nullptr)
		{
			hasClick = ImGui::ImageButton(ToStdString(_id).c_str(), (ImTextureID)_image->getNativeId(), ImVec2(_width, _height), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			hasClick = ImGui::Button(ToStdString(_text).c_str());
		}

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		if (hasClick)
		{
			if (_onClick != nullptr)
			{
				Core::EventHandler::singleton()->addEvent(_onClick);
			}
		}
	}

	void Button::setActive(bool value)
	{
		_active = value;

		if (_active)
			_style.opacity = 1.0f;
		else
			_style.opacity = 0.5f;
	}
}