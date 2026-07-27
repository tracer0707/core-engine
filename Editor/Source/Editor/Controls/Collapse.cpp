#include "Collapse.h"

#include <imgui.h>

namespace Editor
{
	Collapse::Collapse() : Control() {}

	Collapse::Collapse(Core::String text) : Control()
	{
		_text = text;
	}

	Collapse::~Collapse() {}

	void Collapse::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();

		float header_h = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		float total_w = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
		float total_h = header_h;

		bool open = ImGui::CollapsingHeader(_text.std_str().c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		if (open)
		{
			int i = 0;
			for (auto it : _controls)
			{
				it->update();
				total_h += it->getHeight();
				if (it->getWidth() > total_w) total_w = it->getWidth();
			}

			total_h += style.ItemSpacing.y * 2.0f;
		}

		_actualWidth = total_w;
		_actualHeight = total_h;
	}
} // namespace Editor