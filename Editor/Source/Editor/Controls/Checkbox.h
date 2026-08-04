#pragma once

#include "Control.h"

namespace Editor
{
	class Checkbox : public Control
	{
		private:
			bool _value = false;

			std::function<void(bool)> _onValueChanged = nullptr;

		public:
			Checkbox();
			virtual ~Checkbox();

			virtual ControlType getControlType() const { return ControlType::Checkbox; }
			virtual void update();

			bool getValue() const { return _value; }
			void setValue(bool value) { _value = value; }

			void setOnValueChanged(std::function<void(bool)> value) { _onValueChanged = value; }
	};
} // namespace Editor