#include "LinearLayout.h"

#include <imgui_internal.h>

namespace Editor
{
	LinearLayout::LinearLayout() : Control() {}

	LinearLayout::LinearLayout(LayoutDirection direction) : Control()
	{
		_direction = direction;
	}

	LinearLayout::~LinearLayout() {}

	float LinearLayout::getWidth() const
	{
		if (_width == 0.0f)
		{
			return _actualWidth;
		}

		return _width;
	}

	float LinearLayout::getHeight() const
	{
		if (_height == 0.0f)
		{
			return _actualHeight;
		}

		return _height;
	}

	int LinearLayout::getVisibleControlsCount()
	{
		int cnt = 0;
		for (auto& control : _controls)
		{
			if (!control->getVisible()) continue;
			++cnt;
		}
		return cnt;
	}

	void LinearLayout::calculateSizes(std::vector<ImVec2>& sizes)
	{
		sizes.clear();
		sizes.reserve(_controls.count());

		for (auto& control : _controls)
		{
			if (!control->getVisible())
			{
				sizes.push_back(ImVec2(0, 0));
			}
			else
			{
				sizes.push_back(ImVec2(control->getWidth(), control->getHeight()));
			}
		}
	}

	void LinearLayout::calculateWrappedRows(float availableWidth, const std::vector<ImVec2>& sizes, std::vector<RowInfo>& rows)
	{
		rows.clear();

		float currentX = 0.0f;
		RowInfo currentRow;

		ImGuiStyle& style = ImGui::GetStyle();

		for (size_t i = 0; i < sizes.size(); ++i)
		{
			const ImVec2& size = sizes[i];

			if (size.x == 0.0f && size.y == 0.0f)
			{
				continue;
			}

			if (size.x == 0.0f && size.y == 0.0f)
			{
				continue;
			}

			if (currentRow.indices.empty() ||
				(currentRow.totalWidth + size.x + (currentRow.indices.empty() ? 0 : style.ItemSpacing.x) <= availableWidth))
			{
				if (!currentRow.indices.empty())
				{
					currentRow.totalWidth += style.ItemSpacing.x;
				}
				currentRow.indices.push_back(i);
				currentRow.totalWidth += size.x;
				currentRow.maxHeight = std::max(currentRow.maxHeight, size.y);
			}
			else
			{
				if (!currentRow.indices.empty())
				{
					rows.push_back(currentRow);
				}

				currentRow = RowInfo();
				currentRow.indices.push_back(i);
				currentRow.totalWidth = size.x;
				currentRow.maxHeight = size.y;
			}
		}

		if (!currentRow.indices.empty())
		{
			rows.push_back(currentRow);
		}
	}

	void LinearLayout::calculateWrappedColumns(float availableHeight, const std::vector<ImVec2>& sizes, std::vector<ColumnInfo>& columns)
	{
		columns.clear();

		float currentY = 0.0f;
		ColumnInfo currentColumn;

		ImGuiStyle& style = ImGui::GetStyle();

		for (size_t i = 0; i < sizes.size(); ++i)
		{
			const ImVec2& size = sizes[i];

			if (currentColumn.indices.empty() ||
				(currentColumn.totalHeight + size.y + (currentColumn.indices.empty() ? 0 : style.ItemSpacing.y) <= availableHeight))
			{
				if (!currentColumn.indices.empty())
				{
					currentColumn.totalHeight += style.ItemSpacing.y;
				}

				currentColumn.indices.push_back(i);
				currentColumn.totalHeight += size.y;
				currentColumn.maxWidth = std::max(currentColumn.maxWidth, size.x);
			}
			else
			{
				if (!currentColumn.indices.empty())
				{
					columns.push_back(currentColumn);
				}

				currentColumn = ColumnInfo();
				currentColumn.indices.push_back(i);
				currentColumn.totalHeight = size.y;
				currentColumn.maxWidth = size.x;
			}
		}

		if (!currentColumn.indices.empty())
		{
			columns.push_back(currentColumn);
		}
	}

	void LinearLayout::updateHorizontalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize)
	{
		float totalWidth = 0.0f;
		float maxHeight = 0.0f;

		for (const auto& size : sizes)
		{
			totalWidth += size.x;
			maxHeight = std::max(maxHeight, size.y);
		}

		ImGuiStyle& style = ImGui::GetStyle();

		int cntCount = getVisibleControlsCount();

		if (cntCount > 1)
		{
			totalWidth += style.ItemSpacing.x * (cntCount - 1);
		}

		float startX = startPos.x;
		if (_horizontalAlign == LayoutHorizontalAlignment::Center)
		{
			startX += (availableSize.x - totalWidth) * 0.5f;
		}
		else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
		{
			startX += availableSize.x - totalWidth;
		}

		float currentX = startX;

		int placed = 0;
		for (size_t i = 0; i < _controls.count(); ++i)
		{
			Control* c = _controls[i];
			if (!c->getVisible()) continue;

			const ImVec2& size = sizes[i];

			float y = startPos.y;
			if (_verticalAlign == LayoutVerticalAlignment::Middle)
			{
				y += (maxHeight - size.y) * 0.5f;
			}
			else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
			{
				y += maxHeight - size.y;
			}

			ImGui::SetCursorPos(ImVec2(currentX, y));

			c->update();

			currentX += size.x;
			++placed;
			
			if (placed < cntCount)
			{
				currentX += style.ItemSpacing.x;
			}
		}

		if (outSize != nullptr)
		{
			*outSize = ImVec2(totalWidth, maxHeight);
		}
	}

	void LinearLayout::updateHorizontalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize)
	{
		std::vector<RowInfo> rows;
		calculateWrappedRows(availableSize.x, sizes, rows);

		ImGuiStyle& style = ImGui::GetStyle();

		float currentY = startPos.y;

		for (size_t rowIdx = 0; rowIdx < rows.size(); ++rowIdx)
		{
			RowInfo& row = rows[rowIdx];
			row.startY = currentY;

			float startX = startPos.x;
			if (_horizontalAlign == LayoutHorizontalAlignment::Center)
			{
				startX += (availableSize.x - row.totalWidth) * 0.5f;
			}
			else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
			{
				startX += availableSize.x - row.totalWidth;
			}

			float currentX = startX;

			int visibleInRow = 0;
			for (size_t idx : row.indices)
			{
				if (_controls[idx]->getVisible()) ++visibleInRow;
			}

			int placedInRow = 0;
			for (size_t idx : row.indices)
			{
				Control* c = _controls[idx];
				if (!c->getVisible()) continue;

				const ImVec2& size = sizes[idx];

				float y = row.startY;
				if (_verticalAlign == LayoutVerticalAlignment::Middle)
				{
					y += (row.maxHeight - size.y) * 0.5f;
				}
				else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
				{
					y += row.maxHeight - size.y;
				}

				ImGui::SetCursorPos(ImVec2(currentX, y));
				c->update();

				currentX += size.x;
				++placedInRow;
				if (placedInRow < visibleInRow)
				{
					currentX += style.ItemSpacing.x;
				}
			}

			currentY += row.maxHeight;
			if (rowIdx + 1 < rows.size())
			{
				currentY += style.ItemSpacing.y;
			}
		}

		if (outSize != nullptr)
		{
			float fullWidth = 0.0f;
			float fullHeight = 0.0f;
			for (size_t i = 0; i < rows.size(); ++i)
			{
				fullWidth = std::max(fullWidth, rows[i].totalWidth);
				fullHeight += rows[i].maxHeight;
				if (i + 1 < rows.size()) fullHeight += style.ItemSpacing.y;
			}

			*outSize = ImVec2(fullWidth, fullHeight);
		}
	}

	void LinearLayout::updateVerticalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize)
	{
		float totalHeight = 0.0f;
		float maxWidth = 0.0f;

		for (const auto& size : sizes)
		{
			totalHeight += size.y;
			maxWidth = std::max(maxWidth, size.x);
		}

		ImGuiStyle& style = ImGui::GetStyle();

		int cntCount = getVisibleControlsCount();

		if (cntCount > 1)
		{
			totalHeight += style.ItemSpacing.y * (cntCount - 1);
		}

		float startY = startPos.y;
		if (_verticalAlign == LayoutVerticalAlignment::Middle)
		{
			startY += (availableSize.y - totalHeight) * 0.5f;
		}
		else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
		{
			startY += availableSize.y - totalHeight;
		}

		float currentY = startY;

		int placed = 0;
		for (size_t i = 0; i < _controls.count(); ++i)
		{
			Control* c = _controls[i];
			if (!c->getVisible()) continue;

			const ImVec2& size = sizes[i];

			float x = startPos.x;
			if (_horizontalAlign == LayoutHorizontalAlignment::Center)
			{
				x += (maxWidth - size.x) * 0.5f;
			}
			else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
			{
				x += maxWidth - size.x;
			}

			ImGui::SetCursorPos(ImVec2(x, currentY));

			c->update();

			currentY += size.y;
			++placed;
			
			if (placed < cntCount)
			{
				currentY += style.ItemSpacing.y;
			}
		}

		if (outSize != nullptr)
		{
			*outSize = ImVec2(maxWidth, totalHeight);
		}
	}

	void LinearLayout::updateVerticalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize)
	{
		std::vector<ColumnInfo> columns;
		calculateWrappedColumns(availableSize.y, sizes, columns);

		ImGuiStyle& style = ImGui::GetStyle();

		float currentX = startPos.x;

		for (size_t colIdx = 0; colIdx < columns.size(); ++colIdx)
		{
			ColumnInfo& col = columns[colIdx];
			col.startX = currentX;

			float startY = startPos.y;
			if (_verticalAlign == LayoutVerticalAlignment::Middle)
			{
				startY += (availableSize.y - col.totalHeight) * 0.5f;
			}
			else if (_verticalAlign == LayoutVerticalAlignment::Bottom)
			{
				startY += availableSize.y - col.totalHeight;
			}

			float currentY = startY;

			int visibleInCol = 0;
			for (size_t idx : col.indices)
			{
				if (_controls[idx]->getVisible()) ++visibleInCol;
			}

			int placedInCol = 0;
			for (size_t idx : col.indices)
			{
				Control* c = _controls[idx];
				if (!c->getVisible()) continue;

				const ImVec2& size = sizes[idx];

				float x = col.startX;
				if (_horizontalAlign == LayoutHorizontalAlignment::Center)
				{
					x += (col.maxWidth - size.x) * 0.5f;
				}
				else if (_horizontalAlign == LayoutHorizontalAlignment::Right)
				{
					x += col.maxWidth - size.x;
				}

				ImGui::SetCursorPos(ImVec2(x, currentY));
				c->update();

				currentY += size.y;
				++placedInCol;
				if (placedInCol < visibleInCol)
				{
					currentY += style.ItemSpacing.y;
				}
			}

			currentX += col.maxWidth;
			if (colIdx + 1 < columns.size())
			{
				currentX += style.ItemSpacing.x;
			}
		}

		if (outSize != nullptr)
		{
			float fullWidth = 0.0f;
			float fullHeight = 0.0f;
			for (size_t i = 0; i < columns.size(); ++i)
			{
				fullWidth += columns[i].maxWidth;
				if (i + 1 < columns.size()) fullWidth += style.ItemSpacing.x;
				fullHeight = std::max(fullHeight, columns[i].totalHeight);
			}

			*outSize = ImVec2(fullWidth, fullHeight);
		}
	}

	void LinearLayout::update()
	{
		if (!_visible) return;

		uint32_t _flags = ImGuiChildFlags_AlwaysUseWindowPadding;
		uint32_t _windowFlags = ImGuiWindowFlags_None;
		ImGuiStyle& style = ImGui::GetStyle();

		if (_noInputs)
		{
			_windowFlags |= ImGuiWindowFlags_NoInputs;
		}

		float w = _width;
		float h = _height;

		if (_fitWidth == LayoutFitMode::FitContent)
		{
			w = _actualWidth;
		}
		else if (_fitWidth == LayoutFitMode::FitAvailable)
		{
			w = 0;
		}

		if (_fitHeight == LayoutFitMode::FitContent)
		{
			h = _actualHeight;
		}
		else if (_fitHeight == LayoutFitMode::FitAvailable)
		{
			h = 0;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(_style.paddingX, _style.paddingY));
		ImGui::BeginChild(_id.c_str(), ImVec2(w, h), _flags, _windowFlags);

		ImVec2 availableSize = ImVec2(_width, _height);
		ImVec2 cursorStart = ImGui::GetCursorPos();

		if (_fitWidth == LayoutFitMode::FitContent)
		{
			availableSize.x = _actualWidth;
		}
		else if (_fitWidth == LayoutFitMode::FitAvailable)
		{
			availableSize.x = ImGui::GetContentRegionAvail().x;
		}

		if (_fitHeight == LayoutFitMode::FitContent)
		{
			availableSize.y = _actualHeight;
		}
		else if (_fitHeight == LayoutFitMode::FitAvailable)
		{
			availableSize.y = ImGui::GetContentRegionAvail().y;
		}

		std::vector<ImVec2> sizes;
		calculateSizes(sizes);

		ImVec2 finalSize(0, 0);

		if (_direction == LayoutDirection::Horizontal)
		{
			if (_wrapMode == LayoutWrapMode::NoWrap)
			{
				updateHorizontalLayout(cursorStart, availableSize, sizes, &finalSize);
			}
			else
			{
				updateHorizontalLayoutWrapped(cursorStart, availableSize, sizes, &finalSize);
			}
		}
		else
		{
			if (_wrapMode == LayoutWrapMode::NoWrap)
			{
				updateVerticalLayout(cursorStart, availableSize, sizes, &finalSize);
			}
			else
			{
				updateVerticalLayoutWrapped(cursorStart, availableSize, sizes, &finalSize);
			}
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && !ImGui::IsItemClicked(ImGuiMouseButton_Left) &&
			ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			if (_onClick != nullptr)
			{
				_onClick();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();

		updateDragDropSource();
		updateDragDropTarget();

		_actualWidth = finalSize.x;
		_actualHeight = finalSize.y;
	}
} // namespace Editor