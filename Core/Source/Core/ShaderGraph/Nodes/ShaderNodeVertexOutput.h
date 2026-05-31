#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodeVertexOutput : public ShaderNode
	{
			friend class ShaderGraph;

		private:
			ShaderNode* _positionNode = nullptr;

		protected:
			ShaderNodeVertexOutput(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodeVertexOutput() = default;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::VertexOutput; };

			ShaderNode* getPositionNode() const { return _positionNode; }
			void setPositionNode(ShaderNode* node) { _positionNode = node; }
	};
} // namespace Core