#pragma once

#include "Control.h"

namespace Editor
{
	class InputFloat : public Control
	{
		public:
			enum class IncrementType
			{
				Additive,
				Multiplicative
			};

		private:
			float _value = 0.0f;
			float _prevValue = 0.0f;
			float _step = 1.0f;
			float _limitMin = 0.0f;
			float _limitMax = FLT_MAX;

			IncrementType _incrementType = IncrementType::Additive;

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

			float getLimitMin() const { return _limitMin; }
			void setLimitMin(float value) { _limitMin = value; }

			float getLimitMax() const { return _limitMax; }
			void setLimitMax(float value) { _limitMax = value; }

			IncrementType getIncrementType() const { return _incrementType; }
			void setIncrementType(IncrementType value) { _incrementType = value; }

			void setOnValueChanged(std::function<void(float)> value) { _onValueChanged = value; }
	};
} // namespace Editor