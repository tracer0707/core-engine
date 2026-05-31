#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodePosition : public ShaderNode
	{
			friend class ShaderGraph;

		protected:
			ShaderNodePosition(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodePosition() = default;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::Position; };
	};
} // namespace Core