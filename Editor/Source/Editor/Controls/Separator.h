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
		Separator(SeparatorDirection direction);
		virtual ~Separator();

		virtual ControlType getControlType() const { return ControlType::Separator; }
		virtual void update();

		void setSeparatorDirection(SeparatorDirection value) { _direction = value; }
		SeparatorDirection getSeparatorDirection() const { return _direction; }
	};
}