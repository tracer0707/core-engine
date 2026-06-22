#pragma once

#include "Control.h"

namespace Editor
{
	class InputInt : public Control
	{
		private:

		public:
			InputInt();
			virtual ~InputInt();

			virtual ControlType getControlType() const { return ControlType::InputInt; }
			virtual void update();
	};
} // namespace Editor