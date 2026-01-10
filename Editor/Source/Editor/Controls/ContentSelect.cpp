#include "ContentSelect.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

#include <Core/Content/Content.h>
#include <Core/Content/ContentDatabase.h>

#include "ControlList.h"

namespace fs = std::filesystem;

namespace Editor
{
	ContentSelect::ContentSelect() : Control() {}

	ContentSelect::~ContentSelect()
	{

	}

	float ContentSelect::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float ContentSelect::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	int ContentSelect::getControlType() const
	{
		return CONTROL_CONTENT_SELECT;
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

		ImVec2 total_size(_width > 0 ? _width : -1, _height > 0 ? _height : 24.0f);

		ImGui::PushID(_id.c_str());
		bool hasClick = ImGui::InvisibleButton("##ContentSelect", total_size);
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 size = ImGui::GetItemRectSize();
		ImGui::PopID();

		ImU32 bg_col = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_col, style.FrameRounding);
		
		Core::String label = getContentName();
		draw_list->AddText(pos, ImGui::GetColorU32(ImGuiCol_Text), label.std_str().c_str());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(("CONTENT_" + std::to_string(_contentType)).c_str()))
			{
				_content = reinterpret_cast<Core::Content*>(*(void**)payload->Data);
			}
			ImGui::EndDragDropTarget();
		}

		ImVec2 _actualSize = ImGui::GetItemRectSize();
		_actualWidth = _actualSize.x;
		_actualHeight = _actualSize.y;
	}
} // namespace Editor