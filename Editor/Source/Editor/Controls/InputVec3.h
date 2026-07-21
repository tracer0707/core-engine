#pragma once

#include "Control.h"

#include <glm/vec3.hpp>

namespace Editor
{
	class InputVec3 : public Control
	{
		private:
			glm::vec3 _value = glm::vec3(0.0f);
			glm::vec3 _prevValue = glm::vec3(0.0f);

			std::function<void(glm::vec3)> _onValueChanged = nullptr;

		public:
			InputVec3();
			virtual ~InputVec3();

			virtual ControlType getControlType() const { return ControlType::InputVec3; }
			virtual void update();

			glm::vec3 getValue() const { return _value; }
			void setValue(glm::vec3 value) { _value = value; }

			void setOnValueChanged(std::function<void(glm::vec3)> value) { _onValueChanged = value; }
	};
} // namespace Editor