#pragma once

#include "ShaderNode.h"
#include "ShaderNodeTypes.h"

namespace Core
{
	class ShaderNodeVertexOutput : public ShaderNode
	{
			friend class ShaderGraph;

		protected:
			ShaderNodeVertexOutput(const String& varName) : ShaderNode(varName) {}
			virtual ~ShaderNodeVertexOutput() = default;

			ShaderNode* _positionNode = nullptr;
			ShaderNode* _normalNode = nullptr;
			ShaderNode* _tangentNode = nullptr;
			ShaderNode* _bitangentNode = nullptr;
			ShaderNode* _binormalNode = nullptr;
			ShaderNode* _uv0Node = nullptr;
			ShaderNode* _uv1Node = nullptr;
			ShaderNode* _color0Node = nullptr;
			ShaderNode* _color1Node = nullptr;

		public:
			virtual ShaderNodeType getType() const { return ShaderNodeType::VertexOutput; };

			ShaderNode* getPositionNode() const { return _positionNode; }
			void setPositionNode(ShaderNode* node) { _positionNode = node; }

			ShaderNode* getNormalNode() const { return _normalNode; }
			void setNormalNode(ShaderNode* node) { _normalNode = node; }

			ShaderNode* getTangentNode() const { return _tangentNode; }
			void setTangentNode(ShaderNode* node) { _tangentNode = node; }

			ShaderNode* getBitangentNode() const { return _bitangentNode; }
			void setBitangentNode(ShaderNode* node) { _bitangentNode = node; }

			ShaderNode* getBinormalNode() const { return _binormalNode; }
			void setBinormalNode(ShaderNode* node) { _binormalNode = node; }

			ShaderNode* getUv0Node() const { return _uv0Node; }
			void setUv0Node(ShaderNode* node) { _uv0Node = node; }

			ShaderNode* getUv1Node() const { return _uv1Node; }
			void setUv1Node(ShaderNode* node) { _uv1Node = node; }

			ShaderNode* getColor0Node() const { return _color0Node; }
			void setColor0Node(ShaderNode* node) { _color0Node = node; }

			ShaderNode* getColor1Node() const { return _color1Node; }
			void setColor1Node(ShaderNode* node) { _color1Node = node; }
	};
}