#include "Button.h"

#include <imgui.h>

#include <System/EventHandler.h>

#include <Assets/Texture.h>

namespace Editor
{
	Button::Button() : Control() {}

	Button::Button(UString text) : Control()
	{
		_text = text;
	}

	Button::~Button() {}

	void Button::update()
	{
		bool hasClick = false;

		if (_image != nullptr)
		{
			hasClick = ImGui::ImageButton(ToStdString(_text).c_str(), (ImTextureID)_image->getNativeId(), ImVec2(_width, _height), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			hasClick = ImGui::Button(ToStdString(_text).c_str());
		}

		if (hasClick)
		{
			if (_onClick != nullptr)
			{
				Core::EventHandler::singleton()->addEvent(_onClick);
			}
		}
	}
}