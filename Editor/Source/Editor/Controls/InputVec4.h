#pragma once

#include "Control.h"

#include <glm/vec4.hpp>

namespace Editor
{
	class InputVec4 : public Control
	{
		private:
			glm::vec4 _value = glm::vec4(0.0f);
			glm::vec4 _prevValue = glm::vec4(0.0f);

			std::function<void(glm::vec4)> _onValueChanged = nullptr;

		public:
			InputVec4();
			virtual ~InputVec4();

			virtual ControlType getControlType() const { return ControlType::InputVec4; }
			virtual void measure() const;
			virtual void update();

			glm::vec4 getValue() const { return _value; }
			void setValue(glm::vec4 value) { _value = value; }

			void setOnValueChanged(std::function<void(glm::vec4)> value) { _onValueChanged = value; }
	};
} // namespace Editor