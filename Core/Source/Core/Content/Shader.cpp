#include "Shader.h"

#include "../Renderer/Renderer.h"

#include "ContentTypes.h"

namespace Core
{
	Shader::Shader(Renderer* renderer, String vertexSrc, String fragmentSrc) : Content()
	{
		_renderer = renderer;
		_nativeId = _renderer->createProgram(vertexSrc, fragmentSrc);
	}

	Shader::~Shader()
	{
		if (_nativeId != nullptr)
		{
			_renderer->deleteProgram(_nativeId);
			_nativeId = nullptr;
		}

		_renderer = nullptr;
	}

	int Shader::getContentType()
	{
		return CONTENT_TYPE_SHADER;
	}

	void Shader::bind()
	{
		_renderer->bindProgram(_nativeId);
	}
} // namespace Core