#include "Shader.h"

namespace Core
{
    Shader::~Shader()
    {
        if (isLoaded())
            unload();
    }

    const void Shader::unload()
    {
        Renderer::singleton()->deleteProgram(nativeId);
        nativeId = nullptr;

        Asset::unload();
    }

    const void Shader::loadFromString(UString vertexSrc, UString fragmentSrc)
    {
        nativeId = Renderer::singleton()->createProgram(vertexSrc, fragmentSrc);

        Asset::load();
    }

    const void Shader::bind()
    {
        Renderer::singleton()->bindProgram(nativeId);
    }
}