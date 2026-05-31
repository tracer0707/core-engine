#include "ShaderCompilerGLSL.h"

#include <iostream>

#include "../../Shared/String.h"

#include "../ShaderGraph.h"
#include "../Nodes/ShaderNode.h"
#include "../Nodes/ShaderNodeVec4.h"
#include "../Nodes/ShaderNodeMultiply.h"
#include "../Nodes/ShaderNodeVertexOutput.h"
#include "../Nodes/ShaderNodeFragmentOutput.h"

namespace Core
{
	void WriteVertexDeclaration(ShaderGraph* graph, String* outSrc)
	{
		for (ShaderNode* node : graph->getNodes())
		{
			if (node->getType() == ShaderNodeType::Vec4)
			{
				ShaderNodeVec4* vec4Node = (ShaderNodeVec4*)node;

				*outSrc += String("vec4 ") + node->getVarName() + " = vec4(" + std::to_string(vec4Node->getValue().x) + ", " +
						   std::to_string(vec4Node->getValue().y) + ", " + std::to_string(vec4Node->getValue().z) + ", " +
						   std::to_string(vec4Node->getValue().w) + ");\n";
			}
			else if (node->getType() == ShaderNodeType::Position)
			{
				// Handle position node
			}
			else if (node->getType() == ShaderNodeType::UV0)
			{
				// Handle UV node
			}
			else if (node->getType() == ShaderNodeType::Color0)
			{
				// Handle color node
			}
		}
	}

	void WriteFragmentDeclaration(ShaderGraph* graph, String* outSrc) {}

	void WriteNode(ShaderNode* node, String* outSrc)
	{
		if (node->getType() == ShaderNodeType::Vec4)
		{
		}
		else if (node->getType() == ShaderNodeType::Multiply)
		{
			ShaderNodeMultiply* multiplyNode = (ShaderNodeMultiply*)node;
			ShaderNode* operandA = multiplyNode->getOperandA();
			ShaderNode* operandB = multiplyNode->getOperandB();
			if (operandA != nullptr && operandB != nullptr)
			{
				WriteNode(operandA, outSrc);
				WriteNode(operandB, outSrc);
				*outSrc += operandA->getVarName() + " * " + operandB->getVarName() + ";\n";
			}
		}
		else if (node->getType() == ShaderNodeType::VertexOutput)
		{
			// Handle vertex output node
		}
		else if (node->getType() == ShaderNodeType::FragmentOutput)
		{
			ShaderNodeFragmentOutput* fragmentOutputNode = (ShaderNodeFragmentOutput*)node;
			if (fragmentOutputNode->getColorNode() != nullptr)
			{
				WriteNode(fragmentOutputNode->getColorNode(), outSrc);
				*outSrc += String("frag_color = ") + fragmentOutputNode->getColorNode()->getVarName() + ";\n";
			}
		}
	}

	void ShaderCompilerGLSL::compile(ShaderGraph* shaderGraph, String* outVertex, String* outFragment)
	{
		String _vertex = R"(
			#version 400
			layout (location = 0) in vec3 position;
			layout (location = 1) in vec2 uv0;
			layout (location = 2) in vec4 color0;
			out vec2 f_uv0;
			out vec4 f_color0;
			uniform mat4 u_viewMtx;
			uniform mat4 u_projMtx;
			uniform mat4 u_modelMtx;
			void main() {
				f_uv0 = uv0;
				f_color0 = color0;
				gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);
			}
		)";

		String _fragment = R"(
		#version 400
		in vec2 f_uv0;
		in vec4 f_color0;
		out vec4 frag_color;
		void main() {
		)";

		WriteVertexDeclaration(shaderGraph, &_vertex);
		WriteNode(shaderGraph->getVertexOutputNode(), &_vertex);

		_vertex += "}";

		WriteFragmentDeclaration(shaderGraph, &_fragment);
		WriteNode(shaderGraph->getFragmentOutputNode(), &_fragment);

		_fragment += "}";

		std::cout << "Vertex Shader:\n" << _vertex.std_str() << std::endl;
		std::cout << "Fragment Shader:\n" << _fragment.std_str() << std::endl;

		*outVertex = _vertex;
		*outFragment = _fragment;
	}
} // namespace Core