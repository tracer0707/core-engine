#pragma once

#include "Control.h"

namespace Editor
{
	enum class LayoutDirection
	{
		Horizontal,
		Vertical,
	};

	class LinearLayout : public Control
	{
	private:
		LayoutDirection _direction = LayoutDirection::Horizontal;

	public:
		LinearLayout();
		LinearLayout(LayoutDirection direction);
		virtual ~LinearLayout();

		virtual void update();

		void setDirection(LayoutDirection value) { _direction = value; }
		LayoutDirection getDirection() { return _direction; }
	};
}