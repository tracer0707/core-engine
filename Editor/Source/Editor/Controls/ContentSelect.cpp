#include "ContentSelect.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

#include <Core/Shared/Path.h>

#include <Core/Content/Content.h>
#include <Core/Content/ContentDatabase.h>

namespace fs = std::filesystem;

namespace Editor
{
	ContentSelect::ContentSelect() : Control() {}

	ContentSelect::~ContentSelect()
	{

	}

	void ContentSelect::measure() const
	{
		ImGuiStyle& style = ImGui::GetStyle();
		_actualWidth = _width > 0.0f ? _width : ImGui::GetContentRegionAvail().x;
		_actualHeight = _height > 0.0f ? _height : ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
	}

	Core::String ContentSelect::getContentName() const
	{
		if (_content != nullptr)
		{
			fs::path path = Core::ContentDatabase::singleton()->getPath(_content->getUuid());
			return Core::Path::toUtf8(path.filename().stem());
		}

		return "[None]";
	}

	void ContentSelect::setContentType(Core::ContentType value)
	{
		_contentType = value;
		int contentTypeInt = static_cast<int>(_contentType);
		setDragDropTarget(true, Core::String("CONTENT_") + std::to_string(contentTypeInt));
	}

	void ContentSelect::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		Core::String label = getContentName();

		float w = getWidth();
		float h = getHeight();

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

		updateDragDropTarget();
	}
} // namespace Editor