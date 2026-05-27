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
	void BuildNode(ShaderNode* node, String* outSrc)
	{
		ShaderNodeFragmentOutput* fragmentOutputNode;

		switch (node->getType())
		{
		case ShaderNodeType::Vec4:
			*outSrc += String("vec4 ") + node->getVarName() + " = vec4(" + std::to_string(node->getVec4Value().x) + ", " +
					   std::to_string(node->getVec4Value().y) + ", " + std::to_string(node->getVec4Value().z) + ", " +
					   std::to_string(node->getVec4Value().w) + ");\n";
			break;
		case ShaderNodeType::Multiply:
			break;
		case ShaderNodeType::VertexOutput:
			break;
		case ShaderNodeType::FragmentOutput:
			fragmentOutputNode = (ShaderNodeFragmentOutput*)node;
			if (fragmentOutputNode->getColorNode() != nullptr)
			{
				BuildNode(fragmentOutputNode->getColorNode(), outSrc);
				*outSrc += String("frag_color = ") + fragmentOutputNode->getColorNode()->getVarName() + ";\n";
			}
			break;
		default:
			break;
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

		BuildNode(shaderGraph->getFragmentOutputNode(), &_fragment);

		_fragment += "}";

		std::cout << "Vertex Shader:\n" << _vertex.std_str() << std::endl;
		std::cout << "Fragment Shader:\n" << _fragment.std_str() << std::endl;

		*outVertex = _vertex;
		*outFragment = _fragment;
	}
} // namespace Core