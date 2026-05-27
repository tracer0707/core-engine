#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodeVec4 : public ShaderNode
	{
			friend class ShaderGraph;

		protected:
			ShaderNodeVec4(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodeVec4() = default;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::Vec4; };
	};
}