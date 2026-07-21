#include "Button.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <Core/Content/Texture2D.h>

#include "ContextMenu.h"

namespace Editor
{
	Button::Button() : Control() {}

	Button::Button(Core::String text) : Control()
	{
		_text = text;
	}

	Button::Button(Core::Texture2D* image)
	{
		_image = image;
	}

	Button::Button(Core::String text, Core::Texture2D* image)
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
			float w = std::max(_width, _imgW);
			float h = std::max(_height, _imgH);

			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 padding = ImVec2(_style.paddingX, _style.paddingY);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 total_size(w, h);
			ImVec2 text_sz(0, 0);

			if (!_text.empty())
			{
				text_sz = ImGui::CalcTextSize(_text.std_str().c_str());
				total_size.x += text_sz.x + 2.0f + padding.x * 2.0f;
			}

			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::PushID(_id.c_str());
			hasClick = ImGui::InvisibleButton("##ImageButtonWithText", total_size);
			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			ImGui::PopID();

			ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
			ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
			
			draw_list->AddRectFilled(pos, ImVec2(pos.x + total_size.x, pos.y + total_size.y), bg_col, style.FrameRounding);

			if (_text.empty())
			{
				ImVec2 img_p(pos.x + (total_size.x - _imgW) * 0.5f, pos.y + (total_size.y - _imgH) * 0.5f);
				draw_list->AddImage((ImTextureID)_image->getNativeId(), img_p, ImVec2(img_p.x + _imgW, img_p.y + _imgH), ImVec2(0, 1),
									ImVec2(1, 0));
			}
			else
			{
				ImVec2 img_p(pos.x + (total_size.x - _imgW - text_sz.x) * 0.5f, pos.y + (total_size.y - _imgH) * 0.5f);
				draw_list->AddImage((ImTextureID)_image->getNativeId(), img_p, ImVec2(img_p.x + _imgW, img_p.y + _imgH), ImVec2(0, 1),
									ImVec2(1, 0));
				draw_list->AddText(ImVec2(img_p.x + _imgW + 2.0f, pos.y + (total_size.y - text_sz.y) * 0.5f), text_col, _text.std_str().c_str());
			}

			_actualWidth = total_size.x;
			_actualHeight = total_size.y;
		}
		else
		{
			hasClick = ImGui::Button(_text.std_str().c_str(), ImVec2(_width, _height));

			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 text_sz = ImGui::CalcTextSize(_text.std_str().c_str());
			_actualWidth = (_width > 0.0f) ? _width : (text_sz.x + style.FramePadding.x * 2.0f);
			_actualHeight = (_height > 0.0f) ? _height : ImGui::GetFrameHeightWithSpacing();
		}

		ImGui::PopStyleVar();

		updateDragDropSource();
		updateDragDropTarget();

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

	void Button::setActive(bool value)
	{
		_active = value;

		if (_active)
			_style.opacity = 1.0f;
		else
			_style.opacity = 0.5f;
	}
} // namespace Editor