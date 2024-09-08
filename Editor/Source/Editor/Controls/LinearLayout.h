#pragma once

#include "Control.h"

namespace Editor
{
	enum class LayoutDirection
	{
		Horizontal,
		Vertical,
	};
	
	enum class LayoutAlignment
	{
		Start,
		Center,
		End
	};

	class LinearLayout : public Control
	{
	private:
		LayoutDirection _direction = LayoutDirection::Horizontal;
		LayoutAlignment _hAlignment = LayoutAlignment::Start;
		LayoutAlignment _vAlignment = LayoutAlignment::Start;

	public:
		LinearLayout();
		LinearLayout(LayoutDirection direction);
		virtual ~LinearLayout();

		virtual void update();

		void setDirection(LayoutDirection value) { _direction = value; }
		LayoutDirection getDirection() { return _direction; }

		void setAlignmentHorizontal(LayoutAlignment value) { _hAlignment = value; }
		LayoutAlignment getAlignmentHorizontal() { return _hAlignment; }

		void setAlignmentVertical(LayoutAlignment value) { _vAlignment = value; }
		LayoutAlignment getAlignmentVertical() { return _vAlignment; }
	};
}