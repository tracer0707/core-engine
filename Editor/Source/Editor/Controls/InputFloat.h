#pragma once

#include "Control.h"

namespace Editor
{
	class InputFloat : public Control
	{
		private:
			float _value = 0.0f;
			float _prevValue = 0.0f;
			float _step = 0.1f;

			std::function<void(float)> _onValueChanged = nullptr;

		public:
			InputFloat();
			virtual ~InputFloat();

			virtual ControlType getControlType() const { return ControlType::InputFloat; }
			virtual void update();

			float getValue() const { return _value; }
			void setValue(float value) { _value = value; }

			float getStep() const { return _step; }
			void setStep(float value) { _step = value; }

			void setOnValueChanged(std::function<void(float)> value) { _onValueChanged = value; }
	};
} // namespace Editor