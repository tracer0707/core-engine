#pragma once

#include "Control.h"

#include <functional>
#include <vector>
#include <imgui.h>

namespace Editor
{
	enum class LayoutDirection
	{
		Horizontal,
		Vertical,
	};

	enum class LayoutHorizontalAlignment
	{
		Left,
		Center,
		Right
	};

	enum class LayoutVerticalAlignment
	{
		Top,
		Middle,
		Bottom
	};

	enum class LayoutWrapMode
	{
		NoWrap,
		Wrap,
		WrapReverse
	};

	enum class LayoutFitMode
	{
		None,
		FitContent,
		FitAvailable,
	};

	class LinearLayout : public Control
	{
		private:
			LayoutDirection _direction = LayoutDirection::Horizontal;
			LayoutHorizontalAlignment _horizontalAlign = LayoutHorizontalAlignment::Left;
			LayoutVerticalAlignment _verticalAlign = LayoutVerticalAlignment::Top;
			LayoutWrapMode _wrapMode = LayoutWrapMode::Wrap;

			ImVec2 _scrollOffset = ImVec2(0.0f, 0.0f);
			float _scrollbarWidth = 8.0f;
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

			LayoutFitMode _fitWidth = LayoutFitMode::None;
			LayoutFitMode _fitHeight = LayoutFitMode::None;

			bool _noInputs = false;

			struct RowInfo
			{
					std::vector<size_t> indices;
					float totalWidth = 0.0f;
					float maxHeight = 0.0f;
					float startY = 0.0f;
			};

			struct ColumnInfo
			{
					std::vector<size_t> indices;
					float totalHeight = 0.0f;
					float maxWidth = 0.0f;
					float startX = 0.0f;
			};

			std::function<void()> _onClick = nullptr;

			int getVisibleControlsCount();
			void calculateSizes(std::vector<ImVec2>& sizes);
			void calculateWrappedRows(float availableWidth, const std::vector<ImVec2>& sizes, std::vector<RowInfo>& rows);
			void calculateWrappedColumns(float availableHeight, const std::vector<ImVec2>& sizes, std::vector<ColumnInfo>& columns);
			void updateHorizontalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize);
			void updateHorizontalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize);
			void updateVerticalLayout(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize);
			void updateVerticalLayoutWrapped(ImVec2 startPos, ImVec2 availableSize, const std::vector<ImVec2>& sizes, ImVec2* outSize);

		public:
			LinearLayout();
			LinearLayout(LayoutDirection direction);
			virtual ~LinearLayout();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual ControlType getControlType() const { return ControlType::LinearLayout; }
			virtual void update();

			void setDirection(LayoutDirection value) { _direction = value; }
			LayoutDirection getDirection() const { return _direction; }

			void setHorizontalAlignment(LayoutHorizontalAlignment align) { _horizontalAlign = align; }
			LayoutHorizontalAlignment getHorizontalAlignment() const { return _horizontalAlign; }

			void setVerticalAlignment(LayoutVerticalAlignment align) { _verticalAlign = align; }
			LayoutVerticalAlignment getVerticalAlignment() const { return _verticalAlign; }

			void setWrapMode(LayoutWrapMode mode) { _wrapMode = mode; }
			LayoutWrapMode getWrapMode() const { return _wrapMode; }

			void setFitWidth(LayoutFitMode value) { _fitWidth = value; }
			LayoutFitMode getFitWidth() const { return _fitWidth; }

			void setFitHeight(LayoutFitMode value) { _fitHeight = value; }
			LayoutFitMode getFitHeight() const { return _fitHeight; }

			void setNoInputs(bool value) { _noInputs = value; }
			bool getNoInputs() const { return _noInputs; }

			void setOnClick(std::function<void()> value) { _onClick = value; }
	};
} // namespace Editor