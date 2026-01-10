#include "ContentButton.h"

#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <Core/Content/ContentDatabase.h>
#include <Core/Content/Texture.h>

#include "ControlList.h"
#include "ContextMenu.h"

namespace fs = std::filesystem;

namespace Editor
{
	ContentButton::ContentButton() : Control() {}

	ContentButton::ContentButton(Core::Texture* image)
	{
		_image = image;
	}

	ContentButton::~ContentButton()
	{
		setUseContextMenu(false);
	}

	void ContentButton::setUseContextMenu(bool value)
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

	float ContentButton::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float ContentButton::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	Core::String ContentButton::getContentName() const
	{
		if (_content != nullptr)
		{
			Core::String path = Core::ContentDatabase::singleton()->getPath(_content->getUuid());
			return fs::path(path.std_str()).filename().stem().generic_string();
		}

		return Core::String::Empty;
	}

	void ContentButton::startEdit()
	{
		_edit = true;
		_editValue = Core::String::Empty;

		if (_content != nullptr)
		{
			_editValue = getContentName();
		}
	}

	void ContentButton::update()
	{
		if (!_visible || _image == nullptr) return;

		Core::String contentName = getContentName();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
		}

		float w = _width;
		float h = _height;

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 padding = style.FramePadding;

		std::string label = contentName.std_str();
		std::string editLabel = _editValue.std_str();
		float spacing = (!label.empty() || _edit) ? style.ItemInnerSpacing.y : 0;
		ImVec2 text_size = (!label.empty() || _edit) ? ImGui::CalcTextSize(label.c_str()) : ImVec2(0, 0);

		if (w == 0) w = _image->getWidth() + padding.x * 2.0f;
		if (h == 0) h = _image->getHeight() + spacing + text_size.y + padding.y * 2.0f;

		ImVec2 imgSize(w - padding.x * 2.0f, h - spacing - text_size.y - padding.y * 2.0f);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		if (text_size.x > w) text_size.x = w;
		if (text_size.y > h) text_size.y = h;

		ImVec2 total_size(w, h);
		ImVec2 cur = ImGui::GetCursorPos();

		ImGui::PushID(_id.c_str());
		bool hasClick = ImGui::InvisibleButton("##ContentButton", total_size);
		bool hasDblClick = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
		bool hasRightClick = ImGui::IsItemHovered() && ImGui::IsMouseClicked(1);
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 size = ImGui::GetItemRectSize();
		ImGui::PopID();

		ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_col, style.FrameRounding);

		ImVec2 img_p(pos.x + (size.x - imgSize.x) * 0.5f, pos.y + padding.y);

		draw_list->AddImage((ImTextureID)_image->getNativeId(), img_p, ImVec2(img_p.x + imgSize.x, img_p.y + imgSize.y), ImVec2(0, 1), ImVec2(1, 0));

		ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);

		float lowSize = (size.x - text_size.x) * 0.5f;
		float highSize = (size.x + text_size.x) * 0.5f;
		ImVec2 text_min(pos.x + lowSize, img_p.y + imgSize.y + spacing);
		ImVec2 text_max(pos.x + highSize, text_min.y + text_size.y);

		if (!_edit)
		{
			ImGui::RenderTextEllipsis(draw_list, text_min, text_max, text_max.x, text_max.x, label.c_str(), nullptr, nullptr);

			if (_content != nullptr && ImGui::BeginDragDropSource())
			{
				void* data = reinterpret_cast<void*>(_content);
				ImGui::SetDragDropPayload(("CONTENT_" + std::to_string(_content->getContentType())).c_str(), &data, sizeof(void*));
				ImGui::Text(contentName.std_str().c_str());
				ImGui::EndDragDropSource();
			}
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(cur.x + padding.x, cur.y + imgSize.y + spacing));
			ImGui::SetNextItemWidth(_width - padding.x * 2.0f);
			ImGui::SetScrollHereX();
			ImGui::SetScrollHereY();
			ImGui::SetKeyboardFocusHere(0);
			ImGui::InputText(("##" + _id + "_edit").c_str(), &editLabel);
			_editValue = editLabel;
			if (!ImGui::IsItemHovered() && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)))
			{
				_edit = false;
				if (label.empty())
				{
					if (_onEditCancelled != nullptr)
					{
						_onEditCancelled();
					}
				}
				else if (_onEditComplete != nullptr)
				{
					_onEditComplete(_editValue);
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				_edit = false;
				if (_onEditComplete != nullptr)
				{
					_onEditComplete(_editValue);
				}
			}
			ImGui::SetCursorPos(cur);
		}

		_actualWidth = w;
		_actualHeight = h;

		ImGui::PopStyleVar();

		if (!_style.enabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		if (hasClick && _onClick != nullptr)
		{
			_onClick();
		}

		if (hasRightClick)
		{
			if (_contextMenu != nullptr)
			{
				_contextMenu->open();
			}
		}

		if (hasDblClick)
		{
			if (_onDblClick != nullptr)
			{
				_onDblClick();
			}
		}

		if (_contextMenu != nullptr)
		{
			_contextMenu->update();
		}
	}

	int ContentButton::getControlType() const
	{
		return CONTROL_CONTENT_BUTTON;
	}

	void ContentButton::setActive(bool value)
	{
		_active = value;

		if (_active)
			_style.opacity = 1.0f;
		else
			_style.opacity = 0.5f;
	}
} // namespace Editor