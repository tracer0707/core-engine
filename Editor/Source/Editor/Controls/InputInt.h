#pragma once

#include "Control.h"

namespace Editor
{
	class InputInt : public Control
	{
		private:
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

		public:
			InputInt();
			virtual ~InputInt();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual ControlType getControlType() const { return ControlType::InputInt; }
			virtual void update();
	};
} // namespace Editor