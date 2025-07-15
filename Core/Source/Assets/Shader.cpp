#include "Shader.h"

namespace Core
{
    Shader::~Shader()
    {
        if (nativeId != nullptr)
        {
            Renderer::singleton()->deleteProgram(nativeId);
            nativeId = nullptr;
        }
    }

    const void Shader::loadFromString(UString vertexSrc, UString fragmentSrc)
    {
        nativeId = Renderer::singleton()->createProgram(vertexSrc, fragmentSrc);
    }

    const void Shader::bind()
    {
        Renderer::singleton()->bindProgram(nativeId);
    }
}