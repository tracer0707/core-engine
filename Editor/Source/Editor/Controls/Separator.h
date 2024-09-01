#pragma once

#include "Control.h"

namespace Editor
{
	enum class SeparatorDirection
	{
		Horizontal,
		Vertical,
	};

	class Separator : public Control
	{
	private:
		SeparatorDirection _direction = SeparatorDirection::Horizontal;

	public:
		Separator();
		virtual ~Separator();

		virtual void update();

		void setSeparatorDirection(SeparatorDirection value) { _direction = value; }
		SeparatorDirection getSeparatorDirection() { return _direction; }
	};
}