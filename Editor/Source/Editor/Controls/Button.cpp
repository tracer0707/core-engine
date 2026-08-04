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
		bool hasArrowClick = false;

		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.35f);
		}

		if (_image != nullptr)
		{
			float w = std::max(_width, _imgW);
			float h = std::max(_height, _imgH);

			float arrowSize = 12.0f;
			float arrowScale = 0.7f;

			if (_buttonType == ButtonType::Action)
			{
				w -= arrowSize - 1.0f;
			}

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

			if (_buttonType == ButtonType::Action)
			{
				ImGui::SameLine(0, 0);

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.0f);
				
				ImGuiWindow* window = ImGui::GetCurrentWindow();
				const ImGuiID im_id = window->GetID((_id + "_arrow").c_str());

				ImVec2 size_arrow = ImVec2(arrowSize, total_size.y);
				ImVec2 pos = ImVec2(window->DC.CursorPos.x + size_arrow.x, window->DC.CursorPos.y + size_arrow.y);
				
				const ImRect bb(window->DC.CursorPos, pos);
				ImGui::ItemSize(bb);
				if (!ImGui::ItemAdd(bb, im_id)) return;

				bool hovered, held;
				hasArrowClick = ImGui::ButtonBehavior(bb, im_id, &hovered, &held);

				const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
				window->DrawList->AddRectFilled(bb.Min, bb.Max, col, style.FrameRounding);

				float arrow_sz = ImGui::GetFontSize() * arrowScale;
				ImVec2 arrow_pos = ImVec2(bb.Min.x + ((bb.Max.x - bb.Min.x) * 0.5f) - (arrow_sz * 0.5f) - 2.0f,
										  bb.Min.y + ((bb.Max.y - bb.Min.y) * 0.5f) - (arrow_sz * 0.5f));
				ImGui::RenderArrow(window->DrawList, arrow_pos, ImGui::GetColorU32(ImGuiCol_Text), ImGuiDir_Down, arrowScale);

				total_size.x += size_arrow.x + 1.0f;
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

		if (_buttonType == ButtonType::Normal && hasClick)
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
		else if(_buttonType == ButtonType::Action)
		{
			if (hasClick && _onClick != nullptr)
			{
				_onClick();
			}
			if (hasArrowClick && _contextMenu != nullptr)
			{
				_contextMenu->open();
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