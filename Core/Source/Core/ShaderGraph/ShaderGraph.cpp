#include "ShaderGraph.h"

#include "../Config.h"
#include "../Shared/String.h"
#include "../Renderer/Renderer.h"
#include "../Content/ContentTypes.h"

#include "Nodes/ShaderNodeVertexOutput.h"
#include "Nodes/ShaderNodeFragmentOutput.h"
#include "Nodes/ShaderNodeVec4.h"
#include "Nodes/ShaderNodeMultiply.h"

#if CURRENT_RENDERER == GL4
#include "Compilers/ShaderCompilerGLSL.h"
#endif

namespace Core
{
	ShaderGraph::ShaderGraph(Renderer* renderer) : Content()
	{
		_renderer = renderer;

		_vertexOutputNode = createNode<ShaderNodeVertexOutput*>();
		_fragmentOutputNode = createNode<ShaderNodeFragmentOutput*>();
	}

	ShaderGraph::~ShaderGraph()
	{
		if (_program != nullptr)
		{
			_renderer->deleteProgram(_program);
			_program = nullptr;
		}

		delete _vertexOutputNode;
		delete _fragmentOutputNode;

		_vertexOutputNode = nullptr;
		_fragmentOutputNode = nullptr;
		_renderer = nullptr;
	}

	int ShaderGraph::getContentType()
	{
		return CONTENT_TYPE_SHADER;
	}

	void ShaderGraph::compile()
	{
		String vertexSrc;
		String fragmentSrc;

#if CURRENT_RENDERER == GL4
		ShaderCompilerGLSL::compile(this, &vertexSrc, &fragmentSrc);
#endif

		if (_program != nullptr)
		{
			_renderer->deleteProgram(_program);
		}

		_program = _renderer->createProgram(vertexSrc, fragmentSrc);
	}

	void ShaderGraph::bind()
	{
		_renderer->bindProgram(_program);
	}

	ShaderNode* ShaderGraph::createNode(ShaderNodeType type)
	{
		String varName = "var" + std::to_string(_varCounter++);

		switch (type)
		{
		case ShaderNodeType::Vec4:
			return new ShaderNodeVec4(varName);
		case ShaderNodeType::Multiply:
			return new ShaderNodeMultiply(varName);
		case ShaderNodeType::VertexOutput:
			return new ShaderNodeVertexOutput(varName);
		case ShaderNodeType::FragmentOutput:
			return new ShaderNodeFragmentOutput(varName);
		default:
			return nullptr;
		}
	}
} // namespace Core