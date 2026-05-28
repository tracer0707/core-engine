#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodeMultiply : public ShaderNode
	{
			friend class ShaderGraph;

		private:
			ShaderNode* _operandA = nullptr;
			ShaderNode* _operandB = nullptr;

		protected:
			ShaderNodeMultiply(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodeMultiply() = default;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::Multiply; };

			ShaderNode* getOperandA() const { return _operandA; }
			void setOperandA(ShaderNode* node) { _operandA = node; }

			ShaderNode* getOperandB() const { return _operandB; }
			void setOperandB(ShaderNode* node) { _operandB = node; }
	};
} // namespace Core