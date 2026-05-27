#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodeFragmentOutput : public ShaderNode
	{
			friend class ShaderGraph;

		protected:
			ShaderNodeFragmentOutput(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodeFragmentOutput() = default;

			ShaderNode* _colorNode = nullptr;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::FragmentOutput; };

			ShaderNode* getColorNode() const { return _colorNode; }
			void setColorNode(ShaderNode* node) { _colorNode = node; }
	};
}