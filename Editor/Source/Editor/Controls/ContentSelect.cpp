#include "ContentSelect.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

#include <Core/Content/Content.h>
#include <Core/Content/ContentDatabase.h>

namespace fs = std::filesystem;

namespace Editor
{
	ContentSelect::ContentSelect() : Control() {}

	ContentSelect::~ContentSelect()
	{

	}

	Core::String ContentSelect::getContentName() const
	{
		if (_content != nullptr)
		{
			Core::String path = Core::ContentDatabase::singleton()->getPath(_content->getUuid());
			return fs::path(path.std_str()).filename().stem().generic_string();
		}

		return "[None]";
	}

	void ContentSelect::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		Core::String label = getContentName();

		float w = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
		float h = (_height > 0.0f) ? _height : ImGui::GetFrameHeightWithSpacing();

		ImVec2 total_size(w, h);

		ImVec2 pos = ImGui::GetCursorScreenPos();

		ImGui::PushID(_id.c_str());
		bool hasClick = ImGui::InvisibleButton("##ContentSelect", total_size);
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		ImGui::PopID();

		ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		draw_list->AddRectFilled(pos, ImVec2(pos.x + total_size.x, pos.y + total_size.y), bg_col, style.FrameRounding);
		
		draw_list->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), label.std_str().c_str());

		if (ImGui::BeginDragDropTarget())
		{
			int contentTypeInt = static_cast<int>(_contentType);
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(("CONTENT_" + std::to_string(contentTypeInt)).c_str()))
			{
				_content = reinterpret_cast<Core::Content*>(*(void**)payload->Data);
				if (_onContentChanged != nullptr)
				{
					_onContentChanged(_content);
				}
			}
			ImGui::EndDragDropTarget();
		}

		_actualWidth = total_size.x;
		_actualHeight = total_size.y;
	}
} // namespace Editor