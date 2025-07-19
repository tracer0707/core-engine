#include "Shader.h"

namespace Core
{
    Shader::~Shader()
    {
        if (nativeId != nullptr)
        {
            Renderer::current()->deleteProgram(nativeId);
            nativeId = nullptr;
        }
    }

    const void Shader::loadFromString(UString vertexSrc, UString fragmentSrc)
    {
        nativeId = Renderer::current()->createProgram(vertexSrc, fragmentSrc);
    }

    const void Shader::bind()
    {
        Renderer::current()->bindProgram(nativeId);
    }
}