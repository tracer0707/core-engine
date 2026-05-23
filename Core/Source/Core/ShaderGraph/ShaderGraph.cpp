#include "ShaderGraph.h"

#include "../Config.h"
#include "../Shared/String.h"
#include "../Renderer/Renderer.h"
#include "../Content/ContentTypes.h"

#include "Nodes/ShaderNode.h"
#if CURRENT_RENDERER == GL4
#include "Compilers/ShaderCompilerGLSL.h"
#endif

namespace Core
{
	ShaderGraph::ShaderGraph(Renderer* renderer) : Content()
	{
		_renderer = renderer;

		_vertexOutNode = new ShaderNode();
		_fragmentOutNode = new ShaderNode();
	}

	ShaderGraph::~ShaderGraph()
	{
		if (_program != nullptr)
		{
			_renderer->deleteProgram(_program);
			_program = nullptr;
		}

		delete _vertexOutNode;
		delete _fragmentOutNode;

		_vertexOutNode = nullptr;
		_fragmentOutNode = nullptr;
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
} // namespace Core