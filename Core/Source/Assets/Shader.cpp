#include "Shader.h"

#include "../Renderer/Renderer.h"

namespace Core
{
    Shader::Shader(Renderer* renderer, UString vertexSrc, UString fragmentSrc)
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

    void Shader::bind()
    {
        _renderer->bindProgram(_nativeId);
    }
}