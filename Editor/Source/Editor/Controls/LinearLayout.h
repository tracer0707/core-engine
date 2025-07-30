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

		float _totalWidth = FLT_MAX;
		float _totalHeight = FLT_MAX;

	public:
		LinearLayout();
		LinearLayout(LayoutDirection direction);
		virtual ~LinearLayout();

		virtual void update();

		void setDirection(LayoutDirection value) { _direction = value; }
		LayoutDirection getDirection() { return _direction; }

		void setHorizontalAlignment(LayoutAlignment value) { _hAlignment = value; }
		LayoutAlignment getHorizontalAlignment() { return _hAlignment; }

		void setVerticalAlignment(LayoutAlignment value) { _vAlignment = value; }
		LayoutAlignment getVerticalAlignment() { return _vAlignment; }
	};
}