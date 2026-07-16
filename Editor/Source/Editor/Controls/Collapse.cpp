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

	float Collapse::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float Collapse::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	void Collapse::update()
	{
		if (!_visible) return;

		ImGui::BeginGroup();

		if (ImGui::CollapsingHeader(_text.std_str().c_str(), ImGuiTreeNodeFlags_None))
		{
			for (auto it : _controls)
			{
				it->update();
			}
		}

		ImGui::EndGroup();

		ImVec2 _actualSize = ImGui::GetItemRectSize();
		_actualWidth = _actualSize.x;
		_actualHeight = _actualSize.y;
	}
} // namespace Editor