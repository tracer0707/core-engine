#include "Table.h"

#include <stdexcept>

#include <imgui.h>
#include <imgui_internal.h>

#include "ControlList.h"

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

	int Table::getControlType() const
	{
		return CONTROL_TABLE;
	}

	void Table::update()
	{
		if (!_visible) return;

		if (ImGui::BeginTable(_id.c_str(), _colCount, ImGuiTableFlags_Resizable))
		{
			if (_controls.count() % _colCount != 0)
			{
				throw std::runtime_error("Controls count must fill columns entirely");
			}

			for (int i = 0; i < _controls.count() / _colCount + 1; i += _colCount)
			{
				ImGui::TableNextRow();

				for (int j = 0; j < _colCount; ++j)
				{
					Control* control = _controls[i + j];
					ImGui::TableSetColumnIndex(j);
					control->update();
				}
			}

			ImGui::EndTable();
		}

		ImVec2 _actualSize = ImGui::GetItemRectSize();
		_actualWidth = _actualSize.x;
		_actualHeight = _actualSize.y;
	}
} // namespace Editor