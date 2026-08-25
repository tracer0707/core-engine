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

	void Collapse::measure() const
	{
		ImGuiStyle& style = ImGui::GetStyle();
		_actualWidth = _width > 0.0f ? _width : ImGui::GetContentRegionAvail().x;
		_actualHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
		for (auto it : _controls)
		{
			_actualHeight += it->getHeight();
			_actualWidth = std::max(_actualWidth, it->getWidth());
		}
		if (_controls.count() > 0) _actualHeight += style.ItemSpacing.y;
	}

	void Collapse::update()
	{
		if (!_visible) return;

		ImGuiStyle& style = ImGui::GetStyle();

		bool open = false;
		if (_collapseType == CollapseType::Header)
		{
			open = ImGui::CollapsingHeader(_text.std_str().c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		}
		else
		{
			open = ImGui::TreeNodeEx(_text.std_str().c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		}

		if (open)
		{
			int i = 0;
			for (auto it : _controls)
			{
				it->update();
			}

			if (_collapseType == CollapseType::Node)
			{
				ImGui::TreePop();
			}
		}

	}
} // namespace Editor