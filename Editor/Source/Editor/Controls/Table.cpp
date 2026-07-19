#include "Table.h"

#include <stdexcept>

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Table::Table() {}

	Table::~Table() {}

	float Table::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float Table::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	void Table::update()
	{
		if (!_visible) return;

		if (ImGui::BeginTable(_id.c_str(), _colCount))
		{
			if (_controls.count() % _colCount != 0)
			{
				throw std::runtime_error("Controls must fill columns entirely");
			}

			for (int i = 0, j = 0; i < _controls.count(); ++i, ++j)
			{
				if (i % _colCount == 0)
				{
					ImGui::TableNextRow();
					j = 0;
				}

				Control* control = _controls[i];
				ImGui::TableSetColumnIndex(j);
				ImGui::SetNextItemWidth(-FLT_MIN);
				control->update();
			}

			ImGui::EndTable();
		}

		ImVec2 _actualSize = ImGui::GetItemRectSize();
		_actualWidth = _actualSize.x;
		_actualHeight = _actualSize.y;
	}
} // namespace Editor