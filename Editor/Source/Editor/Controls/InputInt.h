#pragma once

#include "Control.h"

namespace Editor
{
	class InputInt : public Control
	{
		private:
			int _value = 0;
			int _prevValue = 0;
			int _step = 1;

			std::function<void(int)> _onValueChanged = nullptr;

		public:
			InputInt();
			virtual ~InputInt();

			virtual ControlType getControlType() const { return ControlType::InputInt; }
			virtual void measure() const;
			virtual void update();

			int getValue() const { return _value; }
			void setValue(int value) { _value = value; }

			int getStep() const { return _step; }
			void setStep(int value) { _step = value; }

			void setOnValueChanged(std::function<void(int)> value) { _onValueChanged = value; }
	};
} // namespace Editor