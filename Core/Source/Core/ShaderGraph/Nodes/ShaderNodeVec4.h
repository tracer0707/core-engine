#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodeVec4 : public ShaderNode
	{
			friend class ShaderGraph;

		private:
			glm::vec4 _value = glm::vec4(0.0f);

		protected:
			ShaderNodeVec4(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodeVec4() = default;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::Vec4; };

			glm::vec4 getValue() const { return _value; }
			void setValue(glm::vec4 value) { _value = value; }
	};
} // namespace Core