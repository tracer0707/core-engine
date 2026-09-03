#include "ContentButton.h"

#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <Core/Content/Texture2D.h>
#include <Core/Content/ContentDatabase.h>
#include <Core/Shared/Path.h>

#include "ContextMenu.h"

#include "../../Resources/Texture.h"

namespace fs = std::filesystem;

namespace Editor
{
	ContentButton::ContentButton() : Control() {}

	ContentButton::ContentButton(Texture* image)
	{
		_image = image;
	}

	ContentButton::ContentButton(Core::Texture2D* coreImage)
	{
		_coreImage = coreImage;
	}

	void ContentButton::measure() const
	{
		if (_image == nullptr && _coreImage == nullptr) return;
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 padding = style.FramePadding;
		std::string label = getContentName().std_str();
		float spacing = (!label.empty() || _edit) ? style.ItemInnerSpacing.y : 0.0f;
		ImVec2 textSize = (!label.empty() || _edit) ? ImGui::CalcTextSize(label.c_str()) : ImVec2(0, 0);
		_actualWidth = _width == 0 ? (_image ? _image->getWidth() : _coreImage->getWidth()) + padding.x * 2.0f : _width;
		_actualHeight = _height == 0 ? (_image ? _image->getHeight() : _coreImage->getHeight()) + spacing + textSize.y + padding.y * 2.0f : _height;
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

	Core::String ContentButton::getContentName() const
	{
		if (_content != nullptr)
		{
			fs::path path = Core::ContentDatabase::singleton()->getPath(_content->getUuid());
			return Core::Path::toUtf8(path.filename().stem());
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
		if (!_visible || (_image == nullptr && _coreImage == nullptr)) return;

		Core::String contentName = getContentName();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * _style.opacity);

		if (!_style.enabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
		}

		float w = getWidth();
		float h = getHeight();

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 padding = style.FramePadding;

		std::string label = contentName.std_str();
		std::string editLabel = _editValue.std_str();
		float spacing = (!label.empty() || _edit) ? style.ItemInnerSpacing.y : 0;
		ImVec2 text_size = (!label.empty() || _edit) ? ImGui::CalcTextSize(label.c_str()) : ImVec2(0, 0);

		ImVec2 imgSize(w - padding.x * 2.0f, h - spacing - text_size.y - padding.y * 2.0f);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 total_size(w, h);
		ImVec2 cur = ImGui::GetCursorPos();
		ImVec2 pos = ImGui::GetCursorScreenPos();

		ImGui::PushID(_id.c_str());
		bool hasClick = ImGui::InvisibleButton("##ContentButton", total_size);
		bool hasDblClick = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
		bool hasRightClick = ImGui::IsItemHovered() && ImGui::IsMouseClicked(1);
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		ImGui::PopID();

		if (text_size.x > total_size.x)
		{
			int ellipsisW = ImGui::CalcTextSize("...").x;

			Core::String croppedText = "";
			Core::String tempText = label;
			int prefSizeX = 0;
			int c = 0;
			while (prefSizeX < (total_size.x - ellipsisW) - 4 && c < tempText.length())
			{
				croppedText += tempText[c];
				prefSizeX = ImGui::CalcTextSize(croppedText.c_str()).x;
				++c;
			}
			label = (croppedText + "...").std_str();
			text_size.x = prefSizeX + ellipsisW;
		}

		ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
		ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImVec2 img_p(pos.x + (total_size.x - imgSize.x) * 0.5f, pos.y + padding.y);

		draw_list->AddRectFilled(pos, ImVec2(pos.x + total_size.x, pos.y + total_size.y), bg_col, style.FrameRounding);
		draw_list->AddImage((ImTextureID)(_image ? _image->getNativeId() : _coreImage->getNativeId()), img_p, ImVec2(img_p.x + imgSize.x, img_p.y + imgSize.y), ImVec2(0, 1), ImVec2(1, 0));

		float lowSize = (total_size.x - text_size.x) * 0.5f;
		ImVec2 text_min(pos.x + lowSize + 1, img_p.y + imgSize.y + spacing);

		if (!_edit)
		{
			draw_list->AddText(text_min, text_col, label.c_str());

			if (_content != nullptr && ImGui::BeginDragDropSource())
			{
				int contentType = static_cast<int>(_content->getContentType());
				void* data = reinterpret_cast<void*>(_content);
				ImGui::SetDragDropPayload(("CONTENT_" + std::to_string(contentType)).c_str(), &data, sizeof(void*));
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
			if (ImGui::IsKeyPressed(ImGuiKey_Escape) || (!ImGui::IsItemHovered() && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1))))
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

	void ContentButton::setActive(bool value)
	{
		_active = value;

		if (_active)
			_style.opacity = 1.0f;
		else
			_style.opacity = 0.5f;
	}
} // namespace Editor