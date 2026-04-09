#pragma once

#include <glm/vec4.hpp>

#include "ShaderNode.h"

namespace Core
{
	class ShaderNodeVec4 : public ShaderNode
	{
		private:
			glm::vec4 _value;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::Vec4; }

			glm::vec4 getValue() const { return _value; }
			void setValue(glm::vec4 value) { _value = value; }
	};
} // namespace Core