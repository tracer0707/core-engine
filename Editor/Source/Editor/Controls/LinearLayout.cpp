#include "LinearLayout.h"

#include <imgui.h>

namespace Editor
{
	LinearLayout::LinearLayout() : Control() {}

	LinearLayout::LinearLayout(LayoutDirection direction) : Control()
	{
		_direction = direction;
	}

	LinearLayout::~LinearLayout() {}

	void LinearLayout::update()
	{
		for (auto it : _controls)
		{
			it->update();

			if (_direction == LayoutDirection::Horizontal)
			{
				ImGui::SameLine();
			}
			else if (_controls.count() > 1)
			{
				ImGui::NewLine();
			}
		}
	}
}