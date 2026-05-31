#pragma once

#include "../Renderer/Program.h"
#include "../Content/Content.h"
#include "../Shared/List.h"

#include "Nodes/ShaderNodeTypes.h"

namespace Core
{
	class Renderer;
	class ShaderNode;
	class ShaderNodeVec4;
	class ShaderNodeMultiply;
	class ShaderNodeVertexOutput;
	class ShaderNodeFragmentOutput;

	class ShaderGraph : public Content
	{
		private:
			Renderer* _renderer = nullptr;
			Program* _program = nullptr;

			int _varCounter = 0;

			ShaderNodeVertexOutput* _vertexOutputNode;
			ShaderNodeFragmentOutput* _fragmentOutputNode;

			List<ShaderNode*> _nodes;

			ShaderNode* createNode(ShaderNodeType type);

		public:
			ShaderGraph(Renderer* renderer);
			~ShaderGraph();

			virtual int getContentType();

			ShaderNodeVertexOutput* getVertexOutputNode() const { return _vertexOutputNode; }
			ShaderNodeFragmentOutput* getFragmentOutputNode() const { return _fragmentOutputNode; }

			template <typename T>
			T createNode() {};

			const List<ShaderNode*>& getNodes() { return _nodes; }

			void compile();
			void bind();
	};

	template <>
	inline ShaderNodeVertexOutput* ShaderGraph::createNode<ShaderNodeVertexOutput*>()
	{
		return (ShaderNodeVertexOutput*)createNode(ShaderNodeType::VertexOutput);
	}

	template <>
	inline ShaderNodeFragmentOutput* ShaderGraph::createNode<ShaderNodeFragmentOutput*>()
	{
		return (ShaderNodeFragmentOutput*)createNode(ShaderNodeType::FragmentOutput);
	}

	template <>
	inline ShaderNodeVec4* ShaderGraph::createNode<ShaderNodeVec4*>()
	{
		return (ShaderNodeVec4*)createNode(ShaderNodeType::Vec4);
	}

	template <>
	inline ShaderNodeMultiply* ShaderGraph::createNode<ShaderNodeMultiply*>()
	{
		return (ShaderNodeMultiply*)createNode(ShaderNodeType::Multiply);
	}
} // namespace Core