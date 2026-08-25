#include "Table.h"

#include <stdexcept>

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Table::Table() {}

	Table::Table(int colCount)
	{
		_colCount = colCount;
	}

	Table::~Table() {}

	void Table::measure() const
	{
		ImGuiStyle& style = ImGui::GetStyle();
		_actualWidth = _width > 0.0f ? _width : ImGui::GetContentRegionAvail().x;
		_actualHeight = 0.0f;
		for (int i = 0; i < _controls.count(); i += _colCount)
		{
			float rowMax = 0.0f;
			for (int j = 0; j < _colCount && i + j < _controls.count(); ++j)
				rowMax = std::max(rowMax, _controls[i + j]->getHeight());
			_actualHeight += rowMax;
		}
		_actualHeight += style.CellPadding.y * 2.0f * (float)(_controls.count() / _colCount);
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
	}
} // namespace Editor