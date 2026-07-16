#pragma once

#include "Control.h"

namespace Editor
{
	class Dummy : public Control
	{
		public:
			Dummy();
			virtual ~Dummy();

			virtual ControlType getControlType() const { return ControlType::Dropdown; }
			virtual void update();
	};
} // namespace Editor