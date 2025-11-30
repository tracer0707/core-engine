#include "Button.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Content/Texture.h>

#include "ControlList.h"

namespace Editor
{
	Button::Button() : Control()
	{
	}

	Button::Button(Core::String text) : Control()
	{
		_text = text;
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
			hasClick = ImGui::ImageButton(_id.c_str(), (ImTextureID)_image->getNativeId(), ImVec2(_width, _height), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			hasClick = ImGui::Button(_text.std_str().c_str());
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
				_onClick();
			}
		}
	}

	int Button::getControlType()
	{
		return CONTROL_BUTTON;
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