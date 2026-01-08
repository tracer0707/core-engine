#include "Button.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <Core/Content/Texture.h>

#include "ControlList.h"
#include "ContextMenu.h"

namespace Editor
{
	Button::Button() : Control() {}

	Button::Button(Core::String text) : Control()
	{
		_text = text;
	}

	Button::Button(Core::Texture* image)
	{
		_image = image;
	}

	Button::Button(Core::String text, Core::Texture* image)
	{
		_text = text;
		_image = image;
	}

	Button::~Button()
	{
		setUseContextMenu(false);
	}

	void Button::setUseContextMenu(bool value)
	{
		if (value)
		{
			if (_contextMenu == nullptr)
			{
				_contextMenu = new ContextMenu();
			}
		}
		else
		{
			if (_contextMenu != nullptr)
			{
				delete _contextMenu;
				_contextMenu = nullptr;
			}
		}
	}

	float Button::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float Button::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

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
			float w = _width;
			float h = _height;

			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 padding = style.FramePadding;

			if (w == 0) w = _image->getWidth() + padding.x * 2.0f;
			if (h == 0) h = _image->getHeight() + padding.y * 2.0f;

			ImVec2 imgSize(w - padding.x * 2.0f, h - padding.y * 2.0f);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 total_size(w, h);

			ImGui::PushID(_id.c_str());
			hasClick = ImGui::InvisibleButton("##ImageButtonWithText", total_size);
			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			ImVec2 pos = ImGui::GetItemRectMin();
			ImVec2 size = ImGui::GetItemRectSize();
			ImGui::PopID();

			ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

			draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_col, style.FrameRounding);

			ImVec2 img_p(pos.x + (size.x - imgSize.x) * 0.5f, pos.y + padding.y);

			draw_list->AddImage((ImTextureID)_image->getNativeId(), img_p, ImVec2(img_p.x + imgSize.x, img_p.y + imgSize.y), ImVec2(0, 1),
								ImVec2(1, 0));

			_actualWidth = w;
			_actualHeight = h;
		}
		else
		{
			hasClick = ImGui::Button(_text.std_str().c_str(), ImVec2(_width, _height));

			ImVec2 _actualSize = ImGui::GetItemRectSize();
			_actualWidth = _actualSize.x;
			_actualHeight = _actualSize.y;
		}

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		if (hasClick)
		{
			if (_contextMenu != nullptr)
			{
				_contextMenu->open();
			}
			else
			{
				if (_onClick != nullptr)
				{
					_onClick();
				}
			}
		}

		if (_contextMenu != nullptr)
		{
			_contextMenu->update();
		}
	}

	int Button::getControlType() const
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
} // namespace Editor