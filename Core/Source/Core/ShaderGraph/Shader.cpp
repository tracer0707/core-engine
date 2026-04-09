#include "Shader.h"

#include "../Config.h"
#include "../Shared/String.h"
#include "../Renderer/Renderer.h"
#include "../Content/ContentTypes.h"

#include "Nodes/ShaderNodeVec4.h"
#if CURRENT_RENDERER == GL4
#include "Compilers/ShaderCompilerGLSL.h"
#endif

namespace Core
{
	Shader::Shader(Renderer* renderer) : Content()
	{
		_renderer = renderer;

		_vertexNode = new ShaderNodeVec4();
		_fragmentNode = new ShaderNodeVec4();
	}

	Shader::~Shader()
	{
		if (_nativeId != nullptr)
		{
			_renderer->deleteProgram(_nativeId);
			_nativeId = nullptr;
		}

		delete _vertexNode;
		delete _fragmentNode;

		_vertexNode = nullptr;
		_fragmentNode = nullptr;
		_renderer = nullptr;
	}

	int Shader::getContentType()
	{
		return CONTENT_TYPE_SHADER;
	}

	void Shader::compile()
	{
		String vertexSrc;
		String fragmentSrc;

#if CURRENT_RENDERER == GL4
		ShaderCompilerGLSL::compile(this, &vertexSrc, &fragmentSrc);
#endif

		if (_nativeId != nullptr)
		{
			_renderer->deleteProgram(_nativeId);
		}

		_nativeId = _renderer->createProgram(vertexSrc, fragmentSrc);
	}

	void Shader::bind()
	{
		_renderer->bindProgram(_nativeId);
	}
} // namespace Core